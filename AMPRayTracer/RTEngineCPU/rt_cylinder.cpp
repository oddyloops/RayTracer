#include "rt_cylinder.h"
#include "vector_util.h"

using namespace rt_support::geometries;

rt_cylinder::rt_cylinder() {}

rt_cylinder::rt_cylinder(float radius, vector<float> top_center, vector<float> base_center)
{
	m_type = rt_geometry_type::cylinder;
	m_radius = radius;
	m_radius_sq = m_radius * m_radius;
	m_top_center = top_center;
	m_base_center = base_center;
	m_height = vector_util::magnitude(top_center - base_center);
	m_axis_dir = (top_center - base_center) / m_height;
	m_orth_d = -vector_util::dot(m_axis_dir, m_base_center);
	m_curve_section =  m_height / ( 2 * m_radius + m_height);
	m_flat_section = 0.5f * (1 - m_curve_section);

	//approximate horizontal axes directions by crossing vertical axis with y axis
	m_hor_axis_dir =vector_util::normalize( vector_util::cross(m_axis_dir, { 0,1,0 }));
	
	if (vector_util::is_zero(m_hor_axis_dir))
	{
		m_hor_axis_dir = { 1,0,0 }; //x axis
		m_hor_axis_dir_perp = { 0,0,1 }; //z axis
	}
	else {
		m_hor_axis_dir_perp = vector_util::normalize(vector_util::cross(m_hor_axis_dir, m_axis_dir));
	}

}


bool rt_cylinder::intersect(ray& ray, intersection_record& record)
{
	

	float dist1,dist2,dist3,dist4 = FLT_MAX;
	
	//intersection with infinite cylinder
	//At^2 + Bt + C = 0
	//A = |r(d) - (r(d).axis-dir)* axis-dir| ^ 2 http://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf
	float A = vector_util::magnitude_sqr(ray.get_direction() - vector_util::dot(ray.get_direction(), m_axis_dir) * m_axis_dir);
	//let p =(r(o) - base_center) 
	vector<float> p = ray.get_origin() - m_base_center;
	//B = 2(r(d) - (r(d).axis-dir) * axis-dir).( p - (p.axis-dir) * axis-dir)  http://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf
	float B = 2 * vector_util::dot(
		(ray.get_direction() - vector_util::dot(ray.get_direction(),m_axis_dir) * m_axis_dir),
		(p - vector_util::dot(p, m_axis_dir) * m_axis_dir)
	);

	//C = |p - (p.axis-dir) * axis-dir | ^ 2 - R ^ 2  http://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf
	float C = vector_util::magnitude_sqr(p - vector_util::dot(p, m_axis_dir) * m_axis_dir) - m_radius_sq;

	float num = B * B - 4 * A * C;
	if (num < 0)
	{
		//no solution to the quadratic equation
		return false;
	}

	dist1 = (-B -sqrtf(num) ) / (2 * A);
	dist2 = (-B + sqrtf(num)) / (2 * A);
	bool intersect_any = false;
	if (dist1 > 0)
	{
		vector<float> pt1 = ray.get_origin() + dist1 * ray.get_direction();
		float pt1toBase = abs(plane_point_dist(pt1, m_axis_dir, m_base_center));
		float pt1toTop = abs(plane_point_dist(pt1, vector_util::negate(m_axis_dir), m_top_center));
		
		intersect_any = intersect_any || ((pt1toBase < m_height) &&  (pt1toTop < m_height));
		if (!intersect_any)
		{
			dist1 = FLT_MAX;
		}
	}
	else {
		dist1 = FLT_MAX;
	}
	if (dist2 > 0)
	{
		vector<float> pt2 = ray.get_origin() + dist2 * ray.get_direction();
		float pt2toBase = abs(plane_point_dist(pt2, m_axis_dir, m_base_center));
		float pt2toTop = abs(plane_point_dist(pt2, vector_util::negate(m_axis_dir), m_top_center));
		bool intersect_pt_2 = ((pt2toBase < m_height) &&  (pt2toTop < m_height));
		intersect_any = intersect_any || intersect_pt_2;
		if (!intersect_pt_2)
		{
			dist2 = FLT_MAX;
		}
	}
	else
	{
		dist2 = FLT_MAX;
	}

	
	

	vector<float> pt3;
	vector<float> pt4;



	vector<float> n1 = m_axis_dir;
	bool intersects = ray_plane_intersection(ray, n1, m_orth_d, dist3, m_base_center); //check for intersection with the base plane

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
	vector<float> n2 = m_axis_dir;
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

	if (!intersect_any)
	{
		return false;
	}

	//update intersection record with the closest distance
	vector<float> normal; //normal at the point of intersect
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
	vector<float> hitPt = ray.get_origin() + dist * ray.get_direction();
	
	if (dist_index < 3)
	{
		vector<float> pBase = hitPt - m_base_center;
		//project pBase on the axis to find axis point
		float axisPtDist = vector_util::dot(pBase, m_axis_dir);
		vector<float> axisPt = m_base_center + axisPtDist * m_axis_dir;
		normal = vector_util::normalize(hitPt - axisPt);
	}
	else if (dist_index == 3)
	{ //top cap
		normal = m_axis_dir;
	}
	else
	{//bottom cap
		normal = vector_util::negate(m_axis_dir);
	}
	float u, v = 0;
	get_uv(hitPt, { 0 }, normal, dist_index, u, v);
	if (!m_normal_map.is_null())
	{
		normal = vector_util::normalize(get_normal(u, v));
	}

	if (!m_bump_map.is_null())
	{
		//bump the point by a height b along the normal
		float b = m_bump_map.get_value(u, v);
		vector<float> old_hit_point = hitPt;
		hitPt = old_hit_point + b * normal;

		float dist_diff = vector_util::magnitude(hitPt - old_hit_point);
		dist += dist_diff;
	}

	record.update_record(dist, hitPt, normal, ray, m_material_index, get_resource_index(), m_type,u,v);
	return true;

}


bool rt_cylinder::inside_circle(vector<float> point, bool is_top_circle)
{
	vector<float> center = is_top_circle ? m_top_center : m_base_center;
	return vector_util::magnitude_sqr(point - center) <= m_radius_sq;
}

void rt_cylinder::get_uv(vector<float> pt, vector<float> bc,  vector<float> n, int dist_index, float& u, float& v) 
{
	vector<float> p = pt - m_base_center;

	if (dist_index < 3)
	{
		///project p on the cylindrical axis
		float dist = vector_util::dot(p, m_axis_dir);
		vector<float> axis_point = m_base_center + dist * m_axis_dir;
		vector<float> p1 = p - axis_point;
		//project p1 on both horizontal axes
		float a1 = vector_util::dot(p1, m_hor_axis_dir);
		float a2 = vector_util::dot(p1, m_hor_axis_dir_perp);
		

		//around the curves 
		u = atan2(a2, a1) / (2 * PI) + 0.5f; //https://gamedev.stackexchange.com/questions/114412/how-to-get-uv-coordinates-for-sphere-cylindrical-projection
		
		//project p on the axis to get vertical distance
		float v_dist = vector_util::dot(p, m_axis_dir);
		v = (v_dist / m_height * m_curve_section) + m_flat_section;
	}
	else {
		//along the flat surfaces
		
		if (dist_index == 3)
		{
			//top surface
			p = pt - m_top_center;
			float p_size = vector_util::magnitude(p);
			//project p1 on both horizontal axes
			float a1 = vector_util::dot(p, m_hor_axis_dir);
			float a2 = vector_util::dot(p, m_hor_axis_dir_perp);
			u = atan2(a2, a1) / (2 * PI) + 0.5f;
			v =( (1 - p_size / m_radius) * m_flat_section ) + m_curve_section + m_flat_section;
		}
		else
		{
			//bottom surface
			float p_size = vector_util::magnitude(p);
			//project p1 on both horizontal axes
			float a1 = vector_util::dot(p, m_hor_axis_dir);
			float a2 = vector_util::dot(p, m_hor_axis_dir_perp);
			u = atan2(a2, a1) / (2 * PI) + 0.5f;
			v = p_size / m_radius * m_flat_section;
		}
	}
}



vector<float> rt_cylinder::get_position(float u, float v)
{
	
	return { 0 };
}


vector<float> rt_cylinder::get_center()
{
	return (m_top_center - m_base_center) / 2;
}
float rt_cylinder::get_radius()
{
	return m_radius;
}

vector<float> rt_cylinder::get_max()
{
	return { 0 };
}

vector<float> rt_cylinder::get_min()
{
	return { 0 };
}