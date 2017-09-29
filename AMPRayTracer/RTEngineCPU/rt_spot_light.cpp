#include "rt_spot_light.h"
#include "math_util.h"
#include "vector_util.h"

using namespace rt_support::lights;

rt_spot_light::rt_spot_light(vector<float> origin, vector<float> direction, float total_fov, float central_fov, float drop_constant) : rt_light(light_type::SPOT)
{
	if (drop_constant < 0)
	{
		throw exception("Drop constant cannot be negative");
	}

	m_origin = origin;
	m_direction = direction;
	m_total_fov = total_fov;
	m_central_fov = central_fov;
	m_drop_constant = drop_constant;
	m_cos_half_fov = cosf(math_util::deg_to_rad(0.5f * m_total_fov));
	m_cos_half_cent_fov = cosf(math_util::deg_to_rad(0.5f * m_central_fov));
}

rt_spot_light::rt_spot_light(vector<float> origin, vector<float> direction, float total_fov, float central_fov, float drop_constant, vector<float> color) : rt_light(light_type::SPOT)
{
	if (drop_constant < 0)
	{
		throw exception("Drop constant cannot be negative");
	}
	m_origin = origin;
	m_direction = direction;
	m_total_fov = total_fov;
	m_central_fov = central_fov;
	m_drop_constant = drop_constant;
	m_color = color;
	m_cos_half_fov = cosf(math_util::deg_to_rad(0.5f * m_total_fov));
	m_cos_half_cent_fov = cosf(math_util::deg_to_rad(0.5f * m_central_fov));
}

rt_spot_light::rt_spot_light(vector<float> origin, vector<float> direction, float total_fov, float central_fov, float drop_constant,vector<float> color, float range, float att_frac, bool is_real_att) : rt_light(light_type::SPOT)
{
	if (drop_constant < 0)
	{
		throw exception("Drop constant cannot be negative");
	}
	m_origin = origin;
	m_direction = direction;
	m_total_fov = total_fov;
	m_central_fov = central_fov;
	m_drop_constant = drop_constant;
	m_color = color;
	m_range = range;
	m_att_frac = att_frac;
	m_realistic_att = is_real_att;
	m_cos_half_fov = cosf(math_util::deg_to_rad(0.5f * m_total_fov));
	m_cos_half_cent_fov = cosf(math_util::deg_to_rad(0.5f * m_central_fov));
}



float rt_spot_light::percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<rt_triangle>& triangles, vector<float> geomPoint, int exceptGeomIndex)
{
	float percent =	1.0f;

	//determine if it is within the total fov
	ray r = ray(geomPoint, m_origin);
	intersection_record rec;
	//compute cosine of angle between ray and centerline of the spotlight
	float cosine = vector_util::dot(vector_util::negate(r.get_direction()), m_direction);

	float dist = vector_util::magnitude(m_origin - r.get_origin()); //distance between spot light and geometry
	if (cosine > m_cos_half_fov)
	{
		//within the coverage of the spotlight
	

		for (rt_rectangle& rect : rects)
		{
			if (rect.get_resource_index() != exceptGeomIndex)
			{
				if (rect.intersect(r, rec) && rec.get_hit_distance() > 0  && rec.get_hit_distance() < dist )
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

		for (rt_triangle& tri : triangles)
		{
			if (tri.get_resource_index() != exceptGeomIndex)
			{
				if (tri.intersect(r, rec) && rec.get_hit_distance() > 0)
				{
					return 0.0f;
				}
			}
		}
			
		//no blockage
		if (m_range > 0)
		{
			percent = compute_attenuation(dist);
		}
		//is it outside the central max intensity circle
		if (cosine < m_cos_half_cent_fov)
		{
			//compute spotlight penumbra attenuation
			percent *= powf((cosine - m_cos_half_fov) / (m_cos_half_cent_fov - m_cos_half_fov), m_drop_constant);
		}

		return percent;
	}
	return 0.0f;
}