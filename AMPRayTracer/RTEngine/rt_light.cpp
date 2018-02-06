#include "rt_light.h"
#include "rt_directional_light.h"
#include "rt_point_light.h"
#include "rt_spot_light.h"
#include "math_util.h"
using namespace rt_support::lights;

rt_light::rt_light(int type)
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
		m_direction = vector_amp::normalize(direction);
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




void rt_light::construct_directional_light(float_3 direction)
{
	construct_directional_light(direction, float_3(1, 1, 1));
}


void rt_light::construct_directional_light(float_3 direction, float_3 color)
{
	if (m_type == light_type::DIRECTIONAL)
	{
		set_direction(direction);
		set_color(color);
	}
	else 
	{
		throw std::exception("Light was not defined as a directional light");
	}
}


void rt_light::construct_point_light(float_3 origin)
{
	construct_point_light(origin, float_3(1, 1, 1));
}



void rt_light::construct_point_light(float_3 origin, float_3 color)
{
	construct_point_light(origin, color, -1, 0, 0);
}


void rt_light::construct_point_light(float_3 origin, float_3 color, float range, float att_frac, int is_real_att)
{
	if (m_type == light_type::POINT)
	{
		set_origin(origin);
		set_color(color);
		set_range(range);
		set_att_frac(att_frac);
		set_realistic_att(is_real_att);
	}
	else 
	{
		throw std::exception("Light was not defined as a point light");
	}
}



void rt_light::construct_spot_light(float_3 origin, float_3 direction, float total_fov, float central_fov, float drop_constant)
{
	construct_spot_light(origin, direction, total_fov, central_fov, drop_constant, float_3(1,1,1), -1, 0, 0);
}

void rt_light::construct_spot_light(float_3 origin, float_3 direction, float total_fov, float central_fov, float drop_constant, float_3 color)
{
	construct_spot_light(origin, direction, total_fov, central_fov, drop_constant, color, -1, 0, 0);
}


void rt_light::construct_spot_light(float_3 origin, float_3 direction, float total_fov, float central_fov, float drop_constant, float_3 color, float range, float att_frac, int is_real_att)
{
	if (m_type == light_type::SPOT)
	{
		set_origin(origin);
		set_direction(direction);
		set_color(color);
		set_range(range);
		set_att_frac(att_frac);
		set_realistic_att(is_real_att);
		m_total_fov = total_fov;
		m_central_fov = central_fov;
		m_drop_constant = drop_constant;
		m_cos_half_fov = cosf(math_util::deg_to_rad(0.5f * m_total_fov));
		m_cos_half_cent_fov = cosf(math_util::deg_to_rad(0.5f * m_central_fov));

	}
	else
	{
		throw std::exception("Light was not defined as a spot light");
	}
}



float rt_light::percent_light(array_view<rt_geometry, 1>& geom, float_3 geomPoint, int exceptGeomIndex) restrict(amp)
{
	float dist;
	float percent;
	switch (m_type)
	{
	case light_type::DIRECTIONAL:
		return rt_directional_light::percent_light(geom, geomPoint, exceptGeomIndex, m_direction);
		break;
	case light_type::POINT:
		percent = rt_point_light::percent_light(geom, geomPoint, exceptGeomIndex, m_origin, dist);
		break;
	case light_type::SPOT:
		percent = rt_spot_light::percent_light(geom, geomPoint, exceptGeomIndex, m_origin, m_direction, m_cos_half_fov, m_cos_half_cent_fov, m_drop_constant, dist);
		break;
	}

	if (m_range > 0)
	{
		percent *= compute_attenuation(dist);
	}

	return percent;
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