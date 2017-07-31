#include "scene_database.h"

using namespace rt_support;

scene_database::scene_database(array_view<rt_sphere, 1> spheres, array_view<rt_rectangle, 1> rects) restrict(amp, cpu)
{
	m_spheres = spheres;
	m_rects = rects;

}


rt_sphere scene_database::get_sphere(int i) restrict(amp, cpu)
{
	index<1> idx(i);
	return m_spheres(idx);
}


rt_rectangle scene_database::get_rect(int i) restrict(amp, cpu)
{
	index<1> idx(i);
	return m_rects(idx);
}

array_view<rt_sphere, 1>& scene_database::get_all_spheres() restrict(amp, cpu)
{
	return m_spheres;
}

array_view<rt_rectangle, 1>& scene_database::get_all_rects() restrict(amp, cpu)
{
	return m_rects;
}

int scene_database::get_num_spheres() restrict(amp, cpu)
{
	return m_spheres.extent.size();
}

int scene_database::get_num_rects() restrict(amp, cpu)
{
	return m_rects.extent.size();
}