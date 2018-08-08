#include "rt_light.h"
#include "math_util.h"
using namespace rt_support::lights;

rt_light::rt_light(int type)
{
	m_type = type;
	m_range = -1; //set as negative by default to disable attenuation
	m_color = { 1,1,1 }; //set to white by default
}

void rt_light::set_color(vector<float> color)
{
	m_color = color;
}
void rt_light::set_direction(vector<float> direction)
{
	if (m_type == light_type::POINT)
	{
		throw exception("Point lights do not have directions");
	}
	m_direction = direction;
}
void rt_light::set_origin(vector<float> origin)
{
	if (m_type == light_type::DIRECTIONAL || m_type == light_type::AREA)
	{
		throw exception("Directional lights do not have origins");
	}
	m_origin = origin;
}
void rt_light::set_range(float range)
{
	if (m_type == light_type::DIRECTIONAL)
	{
		throw exception("Directional lights do not have a finite range");
	}
	m_range = range;
}
void rt_light::set_att_frac(float att_frac)
{
	if (m_type == light_type::DIRECTIONAL)
	{
		throw exception("Directional lights do attenuate");
	}
	m_att_frac = att_frac;
}
void rt_light::set_realistic_att(bool realistic_att)
{
	if (m_type == light_type::DIRECTIONAL)
	{
		throw exception("Directional lights do attenuate");
	}
	m_realistic_att = realistic_att;
}

vector<float> rt_light::get_color() const
{
	return m_color;
}

vector<float> rt_light::get_direction() const
{
	if (m_type == light_type::POINT)
	{
		throw exception("Point lights do not have directions");
	}
	return m_direction;
}
vector<float> rt_light::get_origin() const
{
	if (m_type == light_type::DIRECTIONAL)
	{
		throw exception("Directional lights do not have origins");
	}
	return m_origin;
}

float rt_light::get_range() const
{
	if (m_type == light_type::DIRECTIONAL)
	{
		throw exception("Directional lights do not have a finite range");
	}
	return m_range;
}

float rt_light::get_att_frac() const
{
	if (m_type == light_type::DIRECTIONAL)
	{
		throw exception("Directional lights do attenuate");
	}
	return m_att_frac;
}

bool rt_light::get_is_realistic_att() const
{
	if (m_type == light_type::DIRECTIONAL)
	{
		throw exception("Directional lights do attenuate");
	}
	return m_realistic_att;
}



float rt_light::percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<rt_triangle>& triangles, vector<rt_plane>& planes, vector<rt_cylinder>& cylinders,  vector<float> geomPoint, int exceptGeomIndex)
{
	return 0.0f;
}


float rt_light::compute_attenuation(float hitDist)
{
	if (m_type == light_type::DIRECTIONAL)
	{
		throw exception("Directional lights do attenuate");
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