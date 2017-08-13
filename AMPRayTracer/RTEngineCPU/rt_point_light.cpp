#include "rt_point_light.h"
#include "math_util.h"
#include "vector_util.h"
using namespace rt_support::lights;

rt_point_light::rt_point_light(vector<float> origin) : rt_light(light_type::POINT)
{
	m_origin = origin;
}

rt_point_light::rt_point_light(vector<float> origin, vector<float> color) : rt_light(light_type::POINT)
{
	m_origin = origin;
	m_color = color;
}


rt_point_light::rt_point_light(vector<float> origin, vector<float> color, float range,float att_frac, bool is_real_att) : rt_light(light_type::POINT)
{
	m_origin = origin;
	m_color = color;
	m_range = range;
	m_att_frac = att_frac;
	m_realistic_att = is_real_att;
}

///<summary>
///Determines the percentage of light a pixel gets based on intersection with the light ray
///</summary>
float rt_point_light::percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<float> geomPoint, int exceptGeomIndex)
{
	ray r(geomPoint, m_origin);
	intersection_record rec;

	float dist = vector_util::magnitude(m_origin - r.get_origin()); //distance between point light and geometry

	for (rt_rectangle& rect : rects)
	{
		if (rect.get_resource_index() != exceptGeomIndex)
		{

			if (rect.intersect(r, rec) &&  rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
			{
				
				return 0.0f;
			}
		}
	}

	for (rt_sphere& sph : spheres)
	{
		if (sph.get_resource_index() != exceptGeomIndex)
		{
			if (sph.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
			{
				return 0.0f;
			}
		}
	}

	if (m_range > 0)
	{
		return compute_attenuation(dist);
	}
	return 1.0f;
}