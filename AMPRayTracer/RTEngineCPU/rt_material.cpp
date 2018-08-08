#include "rt_material.h"

using namespace rt_support::scene_resource_support;

rt_material::rt_material()
{

}

rt_material::rt_material(vector<float> ambient_color, vector<float> diffuse_color)
{
	m_diffuse_color = diffuse_color;
	m_ambient_color = ambient_color;
	
}

rt_material::rt_material(vector<float> ambient_color, vector<float> diffuse_color, vector<float> specular_color, float specularity)
{
	m_diffuse_color = diffuse_color;
	m_ambient_color = ambient_color;
	m_specular_color = specular_color;
	m_specularity = specularity;
}


void rt_material::set_ref_properties(float ref_index, float transparency,float reflectivity)
{
	m_refractive_index = ref_index;
	m_transparency = transparency;
	m_reflectivity = reflectivity;
}



void rt_material::set_diffuse(vector<float> color)
{
	m_diffuse_color = color;
}

void rt_material::set_ambience(vector<float> color)
{
	m_ambient_color = color;
}

void rt_material::set_specular(vector<float> color)
{
	m_specular_color = color;
}

void rt_material::set_specular(vector<float> color, float specularity)
{
	m_specular_color = color;
	m_specularity = specularity;
}

vector<float> rt_material::get_ambient_color() const
{
	return m_ambient_color;
}

vector<float> rt_material::get_diffuse_color() const
{
	return m_diffuse_color;
}

vector<float> rt_material::get_specular_color() const
{
	return m_specular_color;
}

float rt_material::get_specularity() const
{
	return m_specularity;
}

float rt_material::get_refractive_index() const
{
	return m_refractive_index;
}

float rt_material::get_transparency() const
{
	return m_transparency;
}

float rt_material::get_reflectivity() const
{
	return m_reflectivity;
}

bool rt_material::get_is_specular()
{
	return m_specularity >= 0;
}

bool rt_material::get_is_reflective_refractive()
{
	return m_reflectivity >= 0 && m_refractive_index >=0;
}
