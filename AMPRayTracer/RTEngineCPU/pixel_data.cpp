#include "pixel_data.h"
using namespace rt_support;

void pixel_data::set_pixel_data(vector<float> position, vector<float> color, float coverage, float depth, float hitDistance)
{
	m_hit_distance = hitDistance;
	m_pixel_color = color;
	m_pixel_coverage = coverage;
	m_pixel_depth = depth;
	m_pixel_position = position;
}

pixel_data::pixel_data()
{
	set_pixel_data({ 0, 0, 0 }, m_default_color, 0, 0, 0);
}

pixel_data::pixel_data(vector<float> position, float hit_distance)
{
	set_pixel_data(position, m_default_color, 0, 0, hit_distance);
}

pixel_data::pixel_data(vector<float> position, vector<float> color, float coverage, float depth, float hitDistance)
{
	set_pixel_data(position,color, coverage, depth, hitDistance);
}


vector<float> pixel_data::get_default_color()
{
	return m_default_color;
}

vector<float> pixel_data::get_pixel_position()
{
	return m_pixel_position;
}

vector<float> pixel_data::get_pixel_color()
{
	return m_pixel_color;
}

float pixel_data::get_pixel_depth()
{
	return m_pixel_depth;
}

float pixel_data::get_pixel_coverage()
{
	return m_pixel_coverage;
}

float pixel_data::get_hit_distance()
{
	return m_hit_distance;
}