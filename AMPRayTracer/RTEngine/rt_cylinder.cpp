#include "rt_cylinder.h"
#include "vector_amp.h"
#include <amp_math.h>

using namespace concurrency::fast_math;

using namespace rt_support::geometries;

rt_cylinder::rt_cylinder() restrict(amp,cpu) {}

rt_cylinder::rt_cylinder(float radius, float_3 top_center, float_3 base_center) restrict(amp,cpu)
{
	m_type = rt_geometry_type::cylinder;
	m_radius = radius;
	m_radius_sq = m_radius * m_radius;
	m_top_center = top_center;
	m_base_center = base_center;
	m_height = vector_amp::magnitude(top_center - base_center);
	m_axis_dir = (top_center - base_center) / m_height;
	m_orth_d = -vector_amp::dot(m_axis_dir, m_base_center);
	m_curve_section = m_height / (2 * m_radius + m_height);
	m_flat_section = 0.5f * (1 - m_curve_section);

	//approximate horizontal axes directions by crossing vertical axis with y axis
	m_hor_axis_dir = vector_amp::normalize(vector_amp::cross(m_axis_dir, { 0,1,0 }));

	if (vector_amp::is_zero(m_hor_axis_dir) == true)
	{
		m_hor_axis_dir = float_3(1,0,0); //x axis
		m_hor_axis_dir_perp = float_3(0,0,1); //z axis
	}
	else {
		m_hor_axis_dir_perp = vector_amp::normalize(vector_amp::cross(m_hor_axis_dir, m_axis_dir));
	}


}


int rt_cylinder::intersect(ray& ray, intersection_record& record, array_view<float_3, 3>* bitmaps, array_view<float_3, 1>* scalars
	, array_view<float, 3>* f_bitmaps, array_view<float, 1>* f_scalars) restrict(amp)
{


	float dist1, dist2, dist3, dist4 = FLT_MAX;

	//intersection with infinite cylinder
	//At^2 + Bt + C = 0
	//A = |r(d) - (r(d).axis-dir)* axis-dir| ^ 2 http://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf
	float A = vector_amp::magnitude_sqr(ray.get_direction() - vector_amp::dot(ray.get_direction(), m_axis_dir) * m_axis_dir);
	//let p =(r(o) - base_center) 
	float_3 p = ray.get_origin() - m_base_center;
	//B = 2(r(d) - (r(d).axis-dir) * axis-dir).( p - (p.axis-dir) * axis-dir)  http://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf
	float B = 2 * vector_amp::dot(
		(ray.get_direction() - vector_amp::dot(ray.get_direction(), m_axis_dir) * m_axis_dir),
		(p - vector_amp::dot(p, m_axis_dir) * m_axis_dir)
	);

	//C = |p - (p.axis-dir) * axis-dir | ^ 2 - R ^ 2  http://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf
	float C = vector_amp::magnitude_sqr(p - vector_amp::dot(p, m_axis_dir) * m_axis_dir) - m_radius_sq;

	float num = B * B - 4 * A * C;
	if (num < 0)
	{
		//no solution to the quadratic equation
		return false;
	}

	dist1 = (-B - sqrtf(num)) / (2 * A);
	dist2 = (-B + sqrtf(num)) / (2 * A);
	int intersect_any = false;
	if (dist1 > 0)
	{
		float_3 pt1 = ray.get_origin() + dist1 * ray.get_direction();
		float pt1toBase = fabsf(plane_point_dist(pt1, m_axis_dir, m_base_center));
		float pt1toTop = fabsf(plane_point_dist(pt1, -m_axis_dir, m_top_center));

		intersect_any = intersect_any || ((pt1toBase < m_height) && (pt1toTop < m_height));
		if (intersect_any == false)
		{
			dist1 = FLT_MAX;
		}
	}
	else {
		dist1 = FLT_MAX;
	}
	if (dist2 > 0)
	{
		float_3 pt2 = ray.get_origin() + dist2 * ray.get_direction();
		float pt2toBase = plane_point_dist(pt2, m_axis_dir, m_base_center);
		float pt2toTop = fabsf(plane_point_dist(pt2, -m_axis_dir, m_top_center));
		int intersect_2 = ((pt2toBase < m_height) && (pt2toTop < m_height));
		intersect_any = intersect_any || intersect_2;

		if (intersect_2 == false)
		{
			dist2 = FLT_MAX;
		}
	}
	else
	{
		dist2 = FLT_MAX;
	}




	float_3 pt3;
	float_3 pt4;



	float_3 n1 = m_axis_dir;
	unsigned int intersects = ray_plane_intersection(ray, n1, m_orth_d, dist3, m_base_center); //check for intersection with the base plane

	if (intersects)
	{
		pt3 = ray.get_origin() + dist3 * ray.get_direction();

		//test for base cap intersection
		if (!inside_circle(pt3, false))
		{
			//not within the base cap
			dist3 = FLT_MAX; //we dont care about that distance value anymore
		}
		else {
			intersect_any = intersect_any || true;
		}



	}

	float_3 n2 = m_axis_dir;
	intersects = ray_plane_intersection(ray, n2, m_orth_d, dist4, m_top_center); //check for intersection with the top plane
	if (intersects)
	{
		pt4 = ray.get_origin() + dist4 * ray.get_direction();

		if (!inside_circle(pt4, true))
		{
			//not within the top cap
			dist4 = FLT_MAX; //we dont care about that distance value anymore
		}
		else
		{
			intersect_any = intersect_any || true;
		}

	}

	if (intersect_any == false)
	{
		return false;
	}

	//update intersection record with the closest distance
	float_3 normal; //normal at the point of intersect
	float dist;
	int dist_index = 1;
	float curr_min = dist1;
	if (dist2 < curr_min)
	{
		curr_min = dist2;
		dist_index = 2;
	}
	if (dist3 < curr_min)
	{
		curr_min = dist3;
		dist_index = 3;
	}
	if (dist4 < curr_min)
	{
		curr_min = dist4;
		dist_index = 4;
	}
	dist = curr_min;
	float_3 hitPt = ray.get_origin() + dist * ray.get_direction();

	if (dist_index < 3)
	{
		//curved side
		float_3 pBase = hitPt - m_base_center;
		//project pBase on the axis to find axis point
		float axisPtDist = vector_amp::dot(pBase, m_axis_dir);
		float_3 axisPt = m_base_center + axisPtDist * m_axis_dir;
		normal = vector_amp::normalize(hitPt - axisPt);
	}
	else if (dist_index == 3)
	{ //top cap
		normal = m_axis_dir;
	}
	else
	{ //bottom cap
		normal = -m_axis_dir;
	}

	float u, v = 0;
	get_uv(hitPt, { 0 }, normal, dist_index, u, v);
	if (!m_normal_map.is_null())
	{
		normal = vector_amp::normalize(get_normal(u, v,bitmaps,scalars));
	}

	if (!m_bump_map.is_null())
	{
		//bump the point by a height b along the normal
		float b = m_bump_map.get_value(u, v,f_bitmaps,f_scalars);
		float_3 old_hit_point = hitPt;
		hitPt = old_hit_point + b * normal;

		float dist_diff = vector_amp::magnitude(hitPt - old_hit_point);
		dist += dist_diff;
	}
	record.update_record(dist, hitPt, normal, ray, m_material_index, get_resource_index(), m_type);
	return true;

}


int rt_cylinder::inside_circle(float_3 point, int is_top_circle) restrict(amp)
{
	float_3 center = is_top_circle ? m_top_center : m_base_center;
	return vector_amp::magnitude_sqr(point - center) <= m_radius_sq;
}

void rt_cylinder::get_uv(float_3 pt, float_3 bc, float_3 n, int dist_index, float& u, float& v) restrict(amp) 
{
	float_3 p = pt - m_base_center;
	const float PI = 3.1415926f;
	if (dist_index < 3)
	{
		///project p on the cylindrical axis
		float dist = vector_amp::dot(p, m_axis_dir);
		float_3 axis_point = m_base_center + dist * m_axis_dir;
		float_3 p1 = p - axis_point;
		//project p1 on both horizontal axes
		float a1 = vector_amp::dot(p1, m_hor_axis_dir);
		float a2 = vector_amp::dot(p1, m_hor_axis_dir_perp);

		
		//around the curves 
		u = atan2(a2, a1) / (2 * 3.142f) + 0.5f; //https://gamedev.stackexchange.com/questions/114412/how-to-get-uv-coordinates-for-sphere-cylindrical-projection

											 //project p on the axis to get vertical distance
		float v_dist = vector_amp::dot(p, m_axis_dir);
		v = (v_dist / m_height * m_curve_section) + m_flat_section;
	}
	else {
		//along the flat surfaces

		if (dist_index == 3)
		{
			//top surface
			p = pt - m_top_center;
			float p_size = vector_amp::magnitude(p);
			//project p1 on both horizontal axes
			float a1 = vector_amp::dot(p, m_hor_axis_dir);
			float a2 = vector_amp::dot(p, m_hor_axis_dir_perp);
			u = atan2(a2, a1) / (2 * PI) + 0.5f;
			v = ((1 - p_size / m_radius) * m_flat_section) + m_curve_section + m_flat_section;
		}
		else
		{
			//bottom surface
			float p_size = vector_amp::magnitude(p);
			//project p1 on both horizontal axes
			float a1 = vector_amp::dot(p, m_hor_axis_dir);
			float a2 = vector_amp::dot(p, m_hor_axis_dir_perp);
			u = atan2(a2, a1) / (2 * PI) + 0.5f;
			v = p_size / m_radius * m_flat_section;
		}
	}
}



float_3 rt_cylinder::get_position(float u, float v) restrict(amp)
{
	return { 0 };
}


float_3 rt_cylinder::get_center() restrict(amp)
{
	return (m_top_center - m_base_center) / 2;
}
float rt_cylinder::get_radius() restrict(amp)
{
	return m_radius;
}

float_3 rt_cylinder::get_max() restrict(amp)
{
	return { 0 };
}

float_3 rt_cylinder::get_min() restrict(amp)
{
	return { 0 };
}