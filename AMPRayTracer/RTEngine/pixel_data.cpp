#include "pixel_data.h"
using namespace rt_support;

void pixel_data::set_pixel_data(float_3 position, float_3 color, float coverage, float depth, float hitDistance) restrict(amp, cpu)
{
	m_hit_distance = hitDistance;
	m_pixel_color = color;
	m_pixel_coverage = coverage;
	m_pixel_depth = depth;
	m_pixel_position = position;
}

pixel_data::pixel_data() restrict(amp, cpu)
{
	set_pixel_data(float_3(0, 0, 0), m_default_color, 0, 0, 0);
}

pixel_data::pixel_data(float_3 position, float hit_distance) restrict(amp, cpu)
{
	set_pixel_data(position, m_default_color, 0, 0, hit_distance);
}

pixel_data::pixel_data(float_3 position, float_3 color, float coverage, float depth, float hitDistance) restrict(amp, cpu)
{
	set_pixel_data(position,color, coverage, depth, hitDistance);
}


float_3 pixel_data::get_default_color() restrict(amp, cpu)
{
	return m_default_color;
}

float_3 pixel_data::get_pixel_position() restrict(amp, cpu)
{
	return m_pixel_position;
}

float_3 pixel_data::get_pixel_color() restrict(amp, cpu)
{
	return m_pixel_color;
}

float pixel_data::get_pixel_depth() restrict(amp, cpu)
{
	return m_pixel_depth;
}

float pixel_data::get_pixel_coverage() restrict(amp, cpu)
{
	return m_pixel_coverage;
}

float pixel_data::get_hit_distance() restrict(amp, cpu)
{
	return m_hit_distance;
}