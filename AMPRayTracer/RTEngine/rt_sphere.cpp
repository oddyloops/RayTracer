
#include <amp_math.h>
#include "rt_sphere.h"
#include "vector_amp.h"

using namespace concurrency::fast_math;
using namespace rt_support::geometries;

rt_sphere::rt_sphere(float_3 center, float radius)
{
	m_center = center;
	m_radius = radius;
	m_radius_squared = m_radius * m_radius;
}

int rt_sphere::intersect(ray& ray, intersection_record& record) restrict(amp)
{
	float_3 l = m_center - ray.get_origin();
	float l_size = vector_amp::magnitude(l);
	//project l on the ray direction
	float ca = vector_amp::dot(l, ray.get_direction());
	if (ca < 0)
		return false;
	float d = sqrtf(l_size * l_size - ca * ca);
	if (d < 0 || d > m_radius)
		return false;

	//compute ray entry point
	float hc = sqrtf(m_radius * m_radius - d * d);
	float_3 pt = ray.get_origin() + (ca - hc)*ray.get_direction(); //adding ca and hc will give exit intersection
	float_3 n = vector_amp::normalize(pt - m_center); //normal at entry intersection
	record.update_record(ca - hc, pt, n, ray, m_material_index, get_resource_index());
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