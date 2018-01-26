#include "math_util.h"
#include "ray.h"

using namespace rt_support::ray_support;


float math_util::deg_to_rad(float deg) restrict(amp, cpu)
{
	return deg * PI / 180;
	
}
 
float math_util::rad_to_degree(float rad) restrict(amp, cpu)
{
	return rad * 180 / PI;
}

float math_util::abs(float x) restrict(amp,cpu)
{
	if (x < 0)
		return -x;
	return x;
}


float math_util::clock_wise_angle(float_3 v1, float_3 v2, float_3 normal) restrict(amp)
{
	float dot = vector_amp::dot(v1, v2);
	float det = v1.x * v2.y * normal.z
		+ v2.x * normal.y * v1.z
		+ normal.x * v1.y * v2.z
		- v1.z * v2.y * normal.x
		- v2.z * normal.y * v1.x
		- normal.z * v1.y * v2.x;
	return atan2f(det, dot);
}


float math_util::plane_point_dist(float_3 pt, float_3 norm, float_3 plane_point) restrict(amp)
{
	float_3 v = pt - plane_point;
	return vector_amp::dot(v, norm);

}


int math_util::ray_plane_intersection(ray& r, float_3& norm, float d, float& dist, float_3 plane_point) restrict(amp)
{
	float denomenator = vector_amp::dot(norm, r.get_direction());

	/*
	* Ray perpendicular to normal
	*/
	if (fabs(denomenator) < FLT_EPSILON)
		return false;

	float numerator = vector_amp::dot(norm, (plane_point - r.get_origin()));
	dist = numerator / denomenator;

	if (denomenator > 0)
		norm = -1 * norm;

	return true;

}


int math_util::inside_circle(float_3 point, float_3 center, float radius_sq) restrict(amp)
{
	float dist_sq = vector_amp::magnitude_sqr(point - center);
	return dist_sq < radius_sq;
}


int math_util::inside_polygon(float_3& pt, float_3 vertices[4], float_3& u_vec, float_3& true_normal) restrict(amp)
{
	float_3 v3 = vertices[3] - vertices[2];
	float_3 v4 = pt - vertices[0];
	float_3 v5 = pt - vertices[2];
	float_3 v1 = u_vec;
	v3 = vector_amp::normalize(v3);
	v4 = vector_amp::normalize(v4);
	v5 = vector_amp::normalize(v5);

	float v1v4 = math_util::clock_wise_angle(v1, v4, true_normal);
	float v3v5 = math_util::clock_wise_angle(v3, v5, true_normal);
	float angle90 = 0.5f * PI;
	return (v1v4 < angle90 && v1v4 > 0) && (v3v5 < angle90 && v3v5 > 0); //clockwise angle between v1 and v4, AND between v3 and v5 must be within 0-90deg

}