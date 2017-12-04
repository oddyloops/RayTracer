#include <amp_math.h>
#include <cfloat>
#include "rt_geometry.h"
#include "vector_amp.h"
#include "matrix_amp.h"
#include "math_util.h"

using namespace concurrency::fast_math;
using namespace rt_support::geometries;


void rt_geometry::set_material_index(int index)
{
	m_material_index = index;
}

float rt_geometry::plane_point_dist(float_3 pt, float_3 norm, float_3 plane_point) restrict(amp)
{
	float_3 v = pt - plane_point;
	return vector_amp::dot(v, norm);

}

int rt_geometry::ray_plane_intersection(ray& r, float_3& norm, float d, float& dist, float_3 plane_point) restrict(amp)
{
	float denomenator = vector_amp::dot(norm, r.get_direction());

	/*
	* Ray perpendicular to normal
	*/
	if (fabs(denomenator) < FLT_EPSILON)
		return false;

	float numerator = vector_amp::dot(norm, (plane_point - r.get_origin())); 
	dist = numerator / denomenator;

	if (denomenator > 0)
		norm = -1 * norm;

	return true;

}

int rt_geometry::intersect(ray& r, intersection_record& record) restrict(amp)
{
	//stub since virtual methods are not allowed
	return false;
}

int rt_geometry::intersect(ray& r, intersection_record& record, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalars, 
	texture<float, 3>* f_bitmaps, texture<float, 1>* f_scalars) restrict(amp)
{
	//stub since virtual methods are not allowed
	return false;
}

float_3 rt_geometry::get_min() restrict(amp,cpu)
{
	return m_min;
}

float_3 rt_geometry::get_max() restrict(amp,cpu)
{
	return m_max;
}

void rt_geometry::get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp)
{
	//stub since virtual methods are not allowed
}

float_3 rt_geometry::get_position(float u, float v) restrict(amp,cpu)
{
	//stub since virtual methods are not allowed
	float_3 blank;
	return blank;
}

int rt_geometry::get_type() restrict(amp,cpu)
{
	return m_type;
}

int rt_geometry::get_material_index() restrict(amp,cpu)
{
	return m_material_index;
}

array<float,2> rt_geometry::parse_xform(float_3 translation, float rx, float ry, float rz, float_3 scale)restrict(cpu)
{
	
	float init[] = { 0,0,0,0,0,0,0,0,0 };
	array<float, 2> rotation(3, 3, init);
	if(rx > FLT_EPSILON)
		rotation = rotation * matrix_amp::create_rotation_x(math_util::deg_to_rad(rx));
	if(ry > FLT_EPSILON)
		rotation = rotation * matrix_amp::create_rotation_y(math_util::deg_to_rad(ry));
	if (rz > FLT_EPSILON)
		rotation = rotation * matrix_amp::create_rotation_z(math_util::deg_to_rad(rz));
	return matrix_amp::create_scale_from_vector(scale) * rotation * matrix_amp::create_translation_from_vector(translation);

}

void rt_geometry::set_normal_map(vector_map normal_map) restrict(amp, cpu)
{
	m_normal_map = normal_map;
}

void rt_geometry::set_bump_map(float_map bump_map) restrict(amp, cpu)
{
	m_bump_map = bump_map;
}

float_3 rt_geometry::get_normal(float u, float v, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalars) restrict(amp)
{
	return m_normal_map.get_value(u, v, bitmaps, scalars);
}