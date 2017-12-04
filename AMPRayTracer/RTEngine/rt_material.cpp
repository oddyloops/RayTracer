#include "rt_material.h"

using namespace rt_support::scene_resource_support;
rt_material::rt_material()  restrict(amp, cpu) {}

rt_material::rt_material(vector_map ambient_color, vector_map diffuse_color) restrict(amp, cpu)
{
	m_diffuse_color = diffuse_color;
	m_ambient_color = ambient_color;
}

rt_material::rt_material(vector_map ambient_color, vector_map diffuse_color, vector_map specular_color, float_map specularity) restrict(amp, cpu)
{
	m_diffuse_color = diffuse_color;
	m_ambient_color = ambient_color;
	m_specular_color = specular_color;
	m_specularity = specularity;
}


void rt_material::set_ref_properties(float_map ref_index, float_map transparency, float_map reflectivity)  restrict(amp, cpu)
{
	m_refractive_index = ref_index;
	m_transparency = transparency;
	m_reflectivity = reflectivity;
}
void rt_material::set_diffuse(vector_map color) restrict(amp, cpu)
{
	m_diffuse_color = color;
}

void rt_material::set_ambience(vector_map color) restrict(amp, cpu)
{
	m_ambient_color = color;
}

void rt_material::set_specular(vector_map color) restrict(amp, cpu)
{
	m_specular_color = color;
}

void rt_material::set_specular(vector_map  color, float_map  specularity)  restrict(amp, cpu)
{
	m_specular_color = color;
	m_specularity = specularity;
}

float_3 rt_material::get_ambient_color(float u, float v, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalars) restrict(amp)
{
	return m_ambient_color.get_value(u,v,bitmaps,scalars);
}

float_3 rt_material::get_diffuse_color(float u, float v, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalars) restrict(amp)
{
	return m_diffuse_color.get_value(u, v, bitmaps, scalars);
}

float_3 rt_material::get_specular_color(float u, float v, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalars) restrict(amp)
{
	return m_specular_color.get_value(u, v, bitmaps, scalars);
}

float rt_material::get_specularity(float u, float v, texture<float, 3>* bitmaps, texture<float, 1>* scalars) restrict(amp)
{
	return m_specularity.get_value(u, v, bitmaps, scalars);
}

float rt_material::get_refractive_index(float u, float v, texture<float, 3>* bitmaps, texture<float, 1>* scalars) restrict(amp)
{
	return m_refractive_index.get_value(u, v, bitmaps, scalars);
}

float rt_material::get_transparency(float u, float v, texture<float, 3>* bitmaps, texture<float, 1>* scalars) restrict(amp)
{
	return m_transparency.get_value(u, v, bitmaps, scalars);
}

float rt_material::get_reflectivity(float u, float v, texture<float, 3>* bitmaps, texture<float, 1>* scalars) restrict(amp)
{
	return m_reflectivity.get_value(u, v, bitmaps, scalars);
}

int rt_material::get_is_specular() restrict(amp)
{
	return m_specular_color.is_null() == false;
}

int rt_material::get_is_reflective_refractive() restrict(amp)
{
	return m_reflectivity.is_null() == false || m_transparency.is_null() == false;
}