#include "rt_directional_light.h"
#include "vector_util.h"

using namespace rt_support::lights;


rt_directional_light::rt_directional_light(vector<float> direction) : rt_light(light_type::DIRECTIONAL)
{
	m_direction = vector_util::normalize(direction);
}

rt_directional_light::rt_directional_light(vector<float> direction, vector<float> color) : rt_light(light_type::DIRECTIONAL)
{
	m_direction = vector_util::normalize(direction);
	m_color = color;
}


float rt_directional_light::percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<rt_triangle>& triangles, vector<rt_plane>& planes, vector<rt_cylinder>& cylinders, vector<float> geomPoint, int exceptGeomIndex)
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


	for (rt_plane& pln : planes)
	{
		if (pln.get_resource_index() != exceptGeomIndex)
		{
			if (pln.intersect(r, rec) && rec.get_hit_distance() > 0)
			{
				return 0.0f;
			}
		}
	}

	for (rt_cylinder& cyl : cylinders)
	{
		if (cyl.get_resource_index() != exceptGeomIndex)
		{
			if (cyl.intersect(r, rec) && rec.get_hit_distance() > 0)
			{
				return 0.0f;
			}
		}
	}


	

	return 1.0f;
}