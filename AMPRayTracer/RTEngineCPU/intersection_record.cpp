#include <cfloat>
#include "intersection_record.h"
#include "rt_core.h"
#include "vector_util.h"


using namespace rt_support::ray_support;


intersection_record::intersection_record()
{
	initialize_record();
}


intersection_record::intersection_record(float hit_dist)
{
	initialize_record();
	m_hit_distance = hit_dist;
}

void intersection_record::initialize_record()
{
	m_bc = { 0, 0, 0 };
	m_geom_index = INVALID_INDEX;
	m_hit_distance = FLT_MAX;
	m_material_index = INVALID_INDEX;
	m_normal = { 0, 1.0f, 0 };
	m_point = { 0, 0, 0 };
	m_ray_dir = { 0, 0, 1.0f };
}

void intersection_record::update_record(float dist, vector<float> intersection_pt, vector<float> normal, ray ray, int mat_index, int geom_index, int type)
{
	if (dist < m_hit_distance)
	{
		m_hit_distance = dist;
		m_point = intersection_pt;
		m_normal = vector_util::normalize(normal);
		m_material_index = mat_index;
		m_geom_index = geom_index;
		m_ray_dir = ray.get_direction();
		m_type = type;
	}

}

void intersection_record::update_bc(float u, float v, float w)
{
	m_bc[0] = u;
	m_bc[1] = v;
	m_bc[2] = w;
}

float intersection_record::get_hit_distance()
{
	return m_hit_distance;
}

int intersection_record::get_material_index()
{
	return m_material_index;
}

void intersection_record::set_material_index(int index)
{
	m_material_index = index;
}

int intersection_record::get_geom_index()
{
	return m_geom_index;
}


void intersection_record::set_geom_index(int index)
{
	m_geom_index = index;
}

vector<float> intersection_record::get_intersection_position()
{
	return m_point;
}

vector<float> intersection_record::get_normal_at_intersect()
{
	return m_normal;
}

vector<float> intersection_record::get_ray_direction()
{
	return m_ray_dir;
}

vector<float> intersection_record::get_hit_pt_bc()
{
	return m_bc;
}

void intersection_record::set_normal_at_intersection(vector<float> n)
{
	m_normal = n;
}

int intersection_record::get_type()
{
	return m_type;
}

void intersection_record::set_type(int type)
{
	m_type = type;
}