#include "rt_directional_light.h"
#include "vector_util.h"

using namespace rt_support::lights;


rt_directional_light::rt_directional_light(vector<float> direction) : rt_light(light_type::DIRECTIONAL)
{
	m_direction = direction;
}

rt_directional_light::rt_directional_light(vector<float> direction, vector<float> color) : rt_light(light_type::DIRECTIONAL)
{
	m_direction = direction;
	m_color = color;
}


float rt_directional_light::percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<float> geomPoint, int exceptGeomIndex)
{
	ray r = ray::create_ray_from_pt_dir(geomPoint, vector_util::negate(m_direction));
	intersection_record rec;
	for (rt_rectangle& rect : rects)
	{
		if (rect.get_resource_index() != exceptGeomIndex)
		{
			if (rect.intersect(r, rec) && rec.get_hit_distance() > 0 )
			{
				return 0.0f;
			}
		}
	}

	for (rt_sphere& sph : spheres)
	{
		if (sph.get_resource_index() != exceptGeomIndex)
		{
			if (sph.intersect(r, rec) && rec.get_hit_distance() > 0)
			{
				return 0.0f;
			}
		}
	}

	return 1.0f;
}