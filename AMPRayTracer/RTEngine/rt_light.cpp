#include "rt_light.h"
#include "math_util.h"
using namespace rt_support::lights;

rt_light::rt_light(int type) restrict(amp,cpu)
{
	m_type = type;
	m_range = -1; //set as negative by default to disable attenuation
	m_color = float_3(1,1,1); //set to white by default
}

void rt_light::set_color(float_3 color) restrict(amp,cpu)
{
	m_color = color;
}
void rt_light::set_direction(float_3 direction) restrict(amp, cpu)
{

	if (m_type != light_type::POINT)
	{
		m_direction = direction;
	}
	
}
void rt_light::set_origin(float_3 origin) restrict(amp, cpu)
{
	if (m_type != light_type::DIRECTIONAL && m_type != light_type::AREA)
	{
		m_origin = origin;
	}
	
}
void rt_light::set_range(float range) restrict(amp, cpu)
{
	if (m_type != light_type::DIRECTIONAL)
	{
		m_range = range;
	}
	
}
void rt_light::set_att_frac(float att_frac) restrict(amp, cpu)
{
	if (m_type != light_type::DIRECTIONAL)
	{
		m_att_frac = att_frac;
	}
	
}
void rt_light::set_realistic_att(int realistic_att) restrict(amp, cpu)
{
	if (m_type != light_type::DIRECTIONAL)
	{
		m_realistic_att = realistic_att;
	}
	
}

float_3 rt_light::get_color() const restrict(amp)
{
	return m_color;
}

float_3 rt_light::get_direction() const restrict(amp)
{
	return m_direction;
}

float_3 rt_light::get_origin() const restrict(amp)
{
	
	return m_origin;
}

float rt_light::get_range() const restrict(amp)
{
	return m_range;
}

float rt_light::get_att_frac() const restrict(amp)
{
	return m_att_frac;
}

int rt_light::get_is_realistic_att() const restrict(amp)
{
	return m_realistic_att;
}



float rt_light::percent_light(array_view<rt_rectangle,1>& rects, array_view<rt_sphere,1>& spheres, 
	array_view<rt_triangle, 1>& triangles, array_view<rt_plane, 1>& planes, array_view<rt_cylinder>& cylinders, float_3 geomPoint, int exceptGeomIndex) restrict(amp)
{
	return 0.0f;
}


float rt_light::compute_attenuation(float hitDist)  restrict(amp)
{
	if (m_type == light_type::DIRECTIONAL)
	{
		return -1;
	}


	if (hitDist < (m_att_frac * m_range))
	{
		//within unattenuated distance
		return 1.0f;
	}
	else if (hitDist > m_range)
	{
		return 0.0f;
	}
	else
	{

		//compute attenuation
		float diff = hitDist - m_att_frac * m_range;
		float max_att_dist = (1.0f - m_att_frac) * m_range;
		float percent = 0;
		if (!m_realistic_att)
		{
			//use linear interpolation

			percent = 1.0f - (diff / max_att_dist);

		}
		else
		{
			//use cosine
			percent = cosf(diff / max_att_dist * math_util::deg_to_rad(90));
		}
		return percent;
	}
}