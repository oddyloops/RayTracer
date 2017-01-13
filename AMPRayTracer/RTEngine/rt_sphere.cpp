
#include <amp_math.h>
#include "rt_sphere.h"
#include "vector_amp.h"

using namespace concurrency::fast_math;
using namespace rt_support::geometries;

rt_sphere::rt_sphere(float_3 center, float radius)
{
	m_center = center;
	radius = radius;
}

int rt_sphere::intersect(ray& ray, intersection_record& record) restrict(amp)
{
	float_3 v1 = ray.get_origin() - m_center;
	float b = 2 * vector_amp::dot(v1, ray.get_direction());
	float c = vector_amp::magnitude_sqr(v1) - m_radius_squared;
	float root = b * b - 4 * c;

	if (root < 0)
		return false;

	root = sqrtf(root);
	float t0 = 0.5f * (-b - root);
	float t1 = 0.5f * (-b + root);

	if ((t0 < 0) && (t1 < 0))
		return false;

	float dist;
	if (t0 < t1)
	{
		if (t0 > 0)
			dist = t0;
		else
			dist = t1;
	}
	else
	{
		if (t1 > 0)
			dist = t1;
		else
			dist = t0;
	}

	if (dist > record.get_hit_distance())
		return false;

	// intersection found
	float_3 pt = ray.get_origin() + dist * ray.get_direction();
	float_3 n = pt - m_center;
	record.update_record(dist, pt, n, ray, m_material_index, get_resource_index());

	return true;

}

void rt_sphere::get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp)
{
	//stubbed
	u = v = 0;
}

float_3 rt_sphere::get_position(float u, float v) restrict(amp)
{
	return float_3();
}

float_3 rt_sphere::get_center() restrict(amp)
{
	return m_center;
}

float rt_sphere::get_radius() restrict(amp)
{
	return m_radius;
}


float_3 rt_sphere::get_max() restrict(amp)
{
	//stub
	return float_3();
}

float_3 rt_sphere::get_min() restrict(amp)
{
	//stub
	return float_3();
}