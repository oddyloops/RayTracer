#include "ray.h"
#include "vector_amp.h"

using namespace rt_support::ray_support;

ray::ray() restrict(amp)
{

}

ray::ray(float_3 o, float_3 at) restrict(amp)
{
	m_origin = o;
	m_direction = vector_amp::normalize(at - o);
}

float_3 ray::get_origin() restrict(amp)
{
	return m_origin;
}

float_3 ray::get_direction() restrict(amp)
{
	return m_direction;
}

ray ray::create_ray_from_pt_dir(float_3 org, float_3 dir) restrict(amp)
{
	ray r;
	r.m_origin = org;
	r.m_direction = dir;
	return r;
}