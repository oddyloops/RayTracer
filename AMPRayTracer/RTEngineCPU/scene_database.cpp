#include "scene_database.h"

using namespace rt_support;

scene_database::scene_database()
{

}


scene_database::scene_database(std::vector<rt_sphere> _spheres,vector<rt_rectangle> _rects, vector<rt_triangle> _triangles,vector<rt_material> materials,
	vector<rt_area_light> area_lights, vector<rt_spot_light> spot_lights, vector<rt_directional_light> directional_lights,
	vector<rt_point_light> point_lights)
{
	m_spheres = _spheres;
	m_rects = _rects;
	m_triangles = _triangles;
	m_materials = materials;
	m_area_lights = area_lights;
	m_point_lights = point_lights;
	m_directional_lights = directional_lights;
	m_spot_lights = spot_lights;
}

void scene_database::add_sphere(rt_sphere s)
{
	m_spheres.push_back(s);
}


void scene_database::add_rect(rt_rectangle r)
{
	m_rects.push_back(r);
}

void scene_database::add_triangle(rt_triangle t)
{
	m_triangles.push_back(t);
}


void scene_database::add_material(rt_material m)
{
	m_materials.push_back(m);
}

void scene_database::add_area_light(rt_area_light a)
{
	m_area_lights.push_back(a);
}

void scene_database::add_directional_light(rt_directional_light d)
{
	m_directional_lights.push_back(d);
}


void scene_database::add_spot_light(rt_spot_light s)
{
	m_spot_lights.push_back(s);
}


void scene_database::add_point_light(rt_point_light p)
{
	m_point_lights.push_back(p);
}

rt_sphere scene_database::get_sphere(int index)
{
	return m_spheres[index];
}

rt_rectangle scene_database::get_rectangle(int index)
{
	return m_rects[index];
}

rt_triangle scene_database::get_triangle(int index)
{
	return m_triangles[index];
}


rt_material scene_database::get_material(int index)
{
	return m_materials[index];
}

rt_area_light scene_database::get_area_light(int index)
{
	return m_area_lights[index];
}


rt_directional_light scene_database::get_directional_light(int index)
{
	return m_directional_lights[index];
}


rt_spot_light scene_database::get_spot_light(int index)
{
	return m_spot_lights[index];
}


rt_point_light scene_database::get_point_light(int index)
{
	return m_point_lights[index];
}

vector<rt_sphere>& scene_database::get_all_spheres()
{
	return m_spheres;
}

vector<rt_rectangle>& scene_database::get_all_rectangles()
{
	return m_rects;
}

vector<rt_triangle>& scene_database::get_all_triangles()
{
	return m_triangles;
}

vector<rt_material>& scene_database::get_all_materials()
{
	return m_materials;
}

vector<rt_directional_light>& scene_database::get_all_directional_lights()
{
	return m_directional_lights;
}

vector<rt_area_light>& scene_database::get_all_area_lights()
{
	return m_area_lights;
}

vector<rt_spot_light>& scene_database::get_spot_lights()
{
	return m_spot_lights;
}

vector<rt_point_light>& scene_database::get_point_lights()
{
	return m_point_lights;
}


int scene_database::get_num_spheres()
{
	return static_cast<int>(m_spheres.size());
}

int scene_database::get_num_rects()
{
	return static_cast<int>(m_rects.size());
}

int scene_database::get_num_triangles()
{
	return m_triangles.size();
}


int scene_database::get_num_materials()
{
	return static_cast<int>(m_materials.size());
}

int scene_database::get_num_area_lights()
{
	return static_cast<int>(m_area_lights.size());
}

int scene_database::get_num_directional_lights()
{
	return static_cast<int>(m_directional_lights.size());
}

int scene_database::get_num_spot_lights()
{
	return static_cast<int>(m_spot_lights.size());
}

int scene_database::get_num_point_lights()
{
	return static_cast<int>(m_point_lights.size());
}
