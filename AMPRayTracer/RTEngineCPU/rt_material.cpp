#include "rt_material.h"

using namespace rt_support::scene_resource_support;

rt_material::rt_material(){ }

rt_material::rt_material(vector<float> diffuse, vector<float> specular)
{
	m_diffuse = diffuse;
	m_specular = specular;
}

vector<float> rt_material::get_diffuse()
{
	return m_diffuse;
}


vector<float> rt_material::get_specular()
{
	return m_specular;
}