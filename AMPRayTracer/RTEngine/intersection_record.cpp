#include <cfloat>
#include "intersection_record.h"
#include "rt_core.h"
#include "vector_amp.h"


using namespace rt_support::ray_support;


intersection_record::intersection_record() restrict(amp)
{
	initialize_record();
}


intersection_record::intersection_record(float hit_dist) restrict(amp)
{
	initialize_record();
	m_hit_distance = hit_dist;
}

void intersection_record::initialize_record() restrict(amp)
{
	m_bc = float_3(0, 0, 0);
	m_geom_index = INVALID_INDEX;
	m_hit_distance = FLT_MAX;
	m_material_index = INVALID_INDEX;
	m_normal = float_3(0, 0, 0);
	m_point = float_3(0, 0, 0);
	m_ray_dir = float_3(0, 0, 0);
}

void intersection_record::update_record(float dist, float_3 intersection_pt, float_3 normal, ray ray, int mat_index, int geom_index, int type) restrict(amp)
{
	if (dist < m_hit_distance && dist > 0)
	{
		m_hit_distance = dist;
		m_point = intersection_pt;
		m_normal = normal;
		m_normal = vector_amp::normalize(m_normal);
		m_material_index = mat_index;
		m_geom_index = geom_index;
		m_ray_dir = ray.get_direction();
		m_type = type;
	}
}

void intersection_record::update_bc(float u, float v, float w) restrict(amp)
{
	m_bc.r = u;
	m_bc.g = v;
	m_bc.b = w;
}

float intersection_record::get_hit_distance() restrict(amp)
{
	return m_hit_distance;
}

int intersection_record::get_material_index() restrict(amp)
{
	return m_material_index;
}

int intersection_record::get_geom_index() restrict(amp)
{
	return m_geom_index;
}

float_3 intersection_record::get_intersection_position() restrict(amp)
{
	return m_point;
}

float_3 intersection_record::get_normal_at_intersect() restrict(amp)
{
	return m_normal;
}

float_3 intersection_record::get_ray_direction() restrict(amp)
{
	return m_ray_dir;
}

float_3 intersection_record::get_hit_pt_bc() restrict(amp)
{
	return m_bc;
}

void intersection_record::set_normal_at_intersection(float_3 n) restrict(amp)
{
	m_normal = n;
}

int intersection_record::get_type() restrict(amp)
{
	return m_type;
}

