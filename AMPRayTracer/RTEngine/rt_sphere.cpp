
#include <amp_math.h>
#include "rt_sphere.h"
#include "vector_amp.h"

using namespace concurrency::fast_math;
using namespace rt_support::geometries;


int rt_sphere::intersect(ray& r, intersection_record& record, float& dist, float_3& pt, float_3& normal, float_3& center, float& radius) restrict(amp)
{
	float_3 l = center - r.get_origin();
	float l_size = vector_amp::magnitude(l);
	//project l on the ray direction
	float ca = vector_amp::dot(l, r.get_direction());
	if (ca < 0)
		return false;
	float d = sqrtf(l_size * l_size - ca * ca);
	if (d < 0 || d > radius)
		return false;

	//compute ray entry point
	float hc = sqrtf(radius * radius - d * d);
	pt = r.get_origin() + (ca - hc)*r.get_direction(); //adding ca and hc will give exit intersection
	normal = vector_amp::normalize(pt - center); //normal at entry intersection
	dist = ca - hc;

	return true;

}



void rt_sphere::get_uv(float_3& pt, float_3& bc, float& u, float& v, float& radius, float_3& base_point, float_3& axis_dir, float_3& hor_axis, float_3& hor_axis_perp) restrict(amp)
{
	const float PI = 3.14159F;
	float_3 pt_base = pt - base_point;
	float_3 axis_point = base_point + (vector_amp::dot(pt_base, axis_dir) * axis_dir);
	float_3 p = pt - axis_point;
	u = (atan2(vector_amp::dot(p, hor_axis), vector_amp::dot(p, hor_axis_perp)) + PI) / (2 * PI); //https://gamedev.stackexchange.com/questions/114412/how-to-get-uv-coordinates-for-sphere-cylindrical-projection
	v = vector_amp::dot(pt_base, axis_dir) / (2 * radius);
}

