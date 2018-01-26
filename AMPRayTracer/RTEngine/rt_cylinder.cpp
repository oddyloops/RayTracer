#include "rt_cylinder.h"
#include "math_util.h"

using namespace concurrency::fast_math;

using namespace rt_support::geometries;


int rt_cylinder::intersect(ray& ray, intersection_record& record, float& hit_distance, float_3& hitPt, float_3& normal, int& dist_index, float& height, float_3& axis_dir, float_3& base_pt, float_3& top_pt, float& radius_sq , float& orth_d
	) restrict(amp)
{


	float dist1, dist2, dist3, dist4 = FLT_MAX;

	//intersection with infinite cylinder
	//At^2 + Bt + C = 0
	//A = |r(d) - (r(d).axis-dir)* axis-dir| ^ 2 http://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf
	float A = vector_amp::magnitude_sqr(ray.get_direction() - vector_amp::dot(ray.get_direction(), axis_dir) * axis_dir);
	//let p =(r(o) - base_center) 
	float_3 p = ray.get_origin() - base_pt;
	//B = 2(r(d) - (r(d).axis-dir) * axis-dir).( p - (p.axis-dir) * axis-dir)  http://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf
	float B = 2 * vector_amp::dot(
		(ray.get_direction() - vector_amp::dot(ray.get_direction(), axis_dir) * axis_dir),
		(p - vector_amp::dot(p, axis_dir) * axis_dir)
	);

	//C = |p - (p.axis-dir) * axis-dir | ^ 2 - R ^ 2  http://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf
	float C = vector_amp::magnitude_sqr(p - vector_amp::dot(p, axis_dir) * axis_dir) - radius_sq;

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
		float pt1toBase = fabsf(math_util::plane_point_dist(pt1, axis_dir, base_pt));
		
		float pt1toTop = fabsf(math_util::plane_point_dist(pt1, -axis_dir, top_pt));

		intersect_any = intersect_any || ((pt1toBase < height) && (pt1toTop < height));
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
		float pt2toBase = math_util::plane_point_dist(pt2, axis_dir, base_pt);
		float pt2toTop = fabsf(math_util::plane_point_dist(pt2, -axis_dir, top_pt));
		int intersect_2 = ((pt2toBase < height) && (pt2toTop < height));
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



	float_3 n1 = axis_dir;
	unsigned int intersects = math_util::ray_plane_intersection(ray, n1, orth_d, dist3, base_pt); //check for intersection with the base plane

	if (intersects)
	{
		pt3 = ray.get_origin() + dist3 * ray.get_direction();

		//test for base cap intersection
		if (math_util::inside_circle(pt3,base_pt, radius_sq) == false)
		{
			//not within the base cap
			dist3 = FLT_MAX; //we dont care about that distance value anymore
		}
		else {
			intersect_any = intersect_any || true;
		}



	}

	float_3 n2 = axis_dir;
	intersects = math_util::ray_plane_intersection(ray, n2, orth_d, dist4, top_pt); //check for intersection with the top plane
	if (intersects)
	{
		pt4 = ray.get_origin() + dist4 * ray.get_direction();

		if (math_util::inside_circle(pt4, top_pt, radius_sq) == false)
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
	hit_distance= curr_min;
	hitPt = ray.get_origin() + hit_distance * ray.get_direction();

	if (dist_index < 3)
	{
		//curved side
		float_3 pBase = hitPt - base_pt;
		//project pBase on the axis to find axis point
		float axisPtDist = vector_amp::dot(pBase, axis_dir);
		float_3 axisPt = base_pt + axisPtDist * axis_dir;
		normal = vector_amp::normalize(hitPt - axisPt);
	}
	else if (dist_index == 3)
	{ //top cap
		normal = axis_dir;
	}
	else
	{ //bottom cap
		normal = -axis_dir;
	}

	return true;

}





void rt_cylinder::get_uv(float_3& pt, float_3& bc, float& u, float& v, int& dist_index, float_3& base_pt, float_3& top_pt, float_3& axis_dir, float_3& hor_axis_1, float_3& hor_axis_2,
	float& curve_section, float& flat_section, float& height, float& radius) restrict(amp)
{
	float_3 p = pt - base_pt;

	if (dist_index < 3)
	{
		///project p on the cylindrical axis
		float dist = vector_amp::dot(p, axis_dir);
		float_3 axis_point = base_pt + dist * axis_dir;
		float_3 p1 = p - axis_point;
		//project p1 on both horizontal axes
		float a1 = vector_amp::dot(p1, hor_axis_1);
		float a2 = vector_amp::dot(p1, hor_axis_2);

		
		//around the curves 
		u = (atan2(a2, a1) + PI) / (2 * PI); //https://gamedev.stackexchange.com/questions/114412/how-to-get-uv-coordinates-for-sphere-cylindrical-projection

											 //project p on the axis to get vertical distance
		float v_dist = vector_amp::dot(p, axis_dir);
		v = (v_dist / height * curve_section) + flat_section;
	}
	else {
		//along the flat surfaces

		if (dist_index == 3)
		{
			//top surface
			p = pt - top_pt;
			float p_size = vector_amp::magnitude(p);
			//project p1 on both horizontal axes
			float a1 = vector_amp::dot(p, hor_axis_1);
			float a2 = vector_amp::dot(p, hor_axis_2);
			u = (atan2(a2, a1) + PI) / (2 * PI);
			v = ((1 - p_size / radius) * flat_section) + curve_section + flat_section;
		}
		else
		{
			//bottom surface
			float p_size = vector_amp::magnitude(p);
			//project p1 on both horizontal axes
			float a1 = vector_amp::dot(p, hor_axis_1);
			float a2 = vector_amp::dot(p, hor_axis_2);
			u = (atan2(a2, a1) + PI) / (2 * PI);
			v = p_size / radius * flat_section;
		}
	}
}



