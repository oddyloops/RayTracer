#include "rt_plane.h"
#include "vector_amp.h"
#include "math_util.h"
using namespace rt_support::geometries;


int rt_plane::intersect(ray& ray, intersection_record& record, float_3& hitPt, float& dist, float_3& normal, float_3& true_normal, float_3& point0, float& d) restrict(amp)
{
	float_3 n = true_normal;
	
	int intersects = math_util::ray_plane_intersection(ray, n, d, dist, point0);
	hitPt = ray.get_origin() + dist * ray.get_direction();
	normal = n;
	return intersects;
}



void rt_plane::get_uv(float_3 pt, float_3 bc, float& u, float& v, float_3& point0, float_3& u_vec, float_3& v_vec, float& map_width, float& map_height) restrict(amp)
{
	float_3 p = pt - point0;
	//project vector p along u and v
	float u_dist = vector_amp::dot(p, u_vec);
	float v_dist = vector_amp::dot(p, v_vec);

	int u_mult = static_cast<int>(math_util::abs(u_dist) / map_width);
	int v_mult = static_cast<int>(math_util::abs(v_dist) / map_height);
	float u_rem = math_util::abs(u_dist) - u_mult * map_width;
	float v_rem = math_util::abs(v_dist) - v_mult * map_height; //used for tiling based on the set map width and height
	u_rem /= map_width;
	v_rem /= map_height;
	if (u_dist < 0)
	{
		u_rem = 1 - u_rem;
	}

	if (v_dist < 0)
	{
		v_rem = 1 - v_rem;
	}

	u = u_rem;
	v = v_rem;
}




