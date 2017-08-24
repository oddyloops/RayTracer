#include "rt_material.h"

using namespace rt_support::scene_resource_support;


rt_material::rt_material(float_3 ambient_color, float_3 diffuse_color) restrict(amp, cpu)
{
	m_diffuse_color = diffuse_color;
	m_ambient_color = ambient_color;
	m_specular_color = float_3(-1, -1, -1);
}

rt_material::rt_material(float_3 ambient_color, float_3 diffuse_color, float_3 specular_color, float specularity) restrict(amp, cpu)
{
	m_diffuse_color = diffuse_color;
	m_ambient_color = ambient_color;
	m_specular_color = specular_color;
	m_specularity = specularity;
}

void rt_material::set_diffuse(float_3 color) restrict(amp, cpu)
{
	m_diffuse_color = color;
}

void rt_material::set_ambience(float_3 color) restrict(amp, cpu)
{
	m_ambient_color = color;
}

void rt_material::set_specular(float_3 color) restrict(amp, cpu)
{
	m_specular_color = color;
}

void rt_material::set_specular(float_3 color, float specularity)  restrict(amp, cpu)
{
	m_specular_color = color;
	m_specularity = specularity;
}

float_3 rt_material::get_ambient_color() const restrict(amp)
{
	return m_ambient_color;
}

float_3 rt_material::get_diffuse_color() const restrict(amp)
{
	return m_diffuse_color;
}

float_3 rt_material::get_specular_color() const restrict(amp)
{
	return m_specular_color;
}

float rt_material::get_specularity() const  restrict(amp)
{
	return m_specularity;
}