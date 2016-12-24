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
	this->m_hit_distance = hit_dist;
}

void intersection_record::initialize_record() restrict(amp)
{
	this->m_bc = float_3(0, 0, 0);
	this->m_geom_index = rt_core::INVALID_INDEX;
	this->m_hit_distance = FLT_MAX;
	this->m_material_index = rt_core::INVALID_INDEX;
	this->m_normal = float_3(0, 0, 0);
	this->m_point = float_3(0, 0, 0);
	this->m_ray_dir = float_3(0, 0, 0);
}

void intersection_record::update_record(float dist, float_3 intersection_pt, float_3 normal, ray ray, int mat_index, int geom_index) restrict(amp)
{
	this->m_hit_distance = dist;
	this->m_point = intersection_pt;
	this->m_normal = normal;
	vector_amp::normalize(this->m_normal);
	this->m_material_index = mat_index;
	this->m_geom_index = geom_index;
	this->m_ray_dir = ray.get_direction();

}

void intersection_record::update_bc(float u, float v, float w) restrict(amp)
{
	this->m_bc.r = u;
	this->m_bc.g = v;
	this->m_bc.b = w;
}

float intersection_record::get_hit_distance() restrict(amp)
{
	return this->m_hit_distance;
}

int intersection_record::get_material_index() restrict(amp)
{
	return this->m_material_index;
}

float_3 intersection_record::get_intersection_position() restrict(amp)
{
	return this->m_point;
}

float_3 intersection_record::get_normal_at_intersect() restrict(amp)
{
	return this->m_normal;
}

float_3 intersection_record::get_ray_direction() restrict(amp)
{
	return this->m_ray_dir;
}

float_3 intersection_record::get_hit_pt_bc() restrict(amp)
{
	return this->m_bc;
}

void intersection_record::set_normal_at_intersection(float_3 n) restrict(amp)
{
	this->m_normal = n;
}

