#include "scene_database.h"

using namespace rt_support;

scene_database::scene_database()
{

}


scene_database::scene_database(std::vector<rt_sphere> _spheres, std::vector<rt_rectangle> _rects)
{
	m_spheres = _spheres;
	m_rects = _rects;
}

void scene_database::add_sphere(rt_sphere s)
{
	m_spheres.push_back(s);
}


void scene_database::add_rect(rt_rectangle r)
{
	m_rects.push_back(r);
}

rt_sphere scene_database::get_sphere(int index)
{
	return m_spheres[index];
}

rt_rectangle scene_database::get_rectangle(int index)
{
	return m_rects[index];
}

vector<rt_sphere>& scene_database::get_all_spheres()
{
	return m_spheres;
}

vector<rt_rectangle>& scene_database::get_all_rectangles()
{
	return m_rects;
}

int scene_database::get_num_spheres()
{
	return static_cast<int>(m_spheres.size());
}

int scene_database::get_num_rects()
{
	return static_cast<int>(m_rects.size());
}