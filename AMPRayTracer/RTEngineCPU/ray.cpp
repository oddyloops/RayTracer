#include "ray.h"
#include "vector_util.h"

using namespace rt_support::ray_support;

ray::ray()
{

}

ray::ray(vector<float> o, vector<float> at)
{
	m_origin = o;
	m_direction = vector_util::normalize(at - o);
}

vector<float> ray::get_origin()
{
	return m_origin;
}

vector<float> ray::get_direction()
{
	return m_direction;
}

ray ray::create_ray_from_pt_dir(vector<float> org, vector<float> dir)
{
	ray r;
	r.m_origin = org;
	r.m_direction = dir;
	return r;
}