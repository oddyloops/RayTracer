#include "rt_directional_light.h"
#include "vector_amp.h"

using namespace rt_support::lights;


rt_directional_light::rt_directional_light(float_3 direction) restrict(amp,cpu) : rt_light(light_type::DIRECTIONAL)
{
	m_direction = vector_amp::normalize(direction);
}

rt_directional_light::rt_directional_light(float_3 direction, float_3 color) restrict(amp, cpu)  : rt_light(light_type::DIRECTIONAL)
{
	m_direction = vector_amp::normalize(direction);
	m_color = color;
}


float rt_directional_light::percent_light(array_view<rt_rectangle,1>& rects, array_view<rt_sphere,1>& spheres,
	array_view<rt_triangle, 1>& triangles, array_view<rt_plane, 1>& planes, array_view<rt_cylinder>& cylinders, float_3 geomPoint, int exceptGeomIndex) restrict(amp)
{
	ray r = ray::create_ray_from_pt_dir(geomPoint, -m_direction);
	intersection_record rec;
	for (int i =0; i < rects.extent.size(); i++)
	{
		index<1> idx(i);
		rt_rectangle& rect = rects(idx);
		if (rect.get_resource_index() != exceptGeomIndex)
		{
			if (rect.intersect(r, rec) && rec.get_hit_distance() > 0)
			{
				return 0.0f;
			}
		}
	}

	for (int i = 0; i < spheres.extent.size(); i++)
	{
		index<1> idx(i);
		rt_sphere& sph = spheres(idx);
		if (sph.get_resource_index() != exceptGeomIndex)
		{
			if (sph.intersect(r, rec) && rec.get_hit_distance() > 0)
			{
				return 0.0f;
			}
		}
	}


	for (int i = 0; i < triangles.extent.size(); i++)
	{
		index<1> idx(i);
		rt_triangle& tri = triangles(idx);
		if (tri.get_resource_index() != exceptGeomIndex)
		{
			if (tri.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() >0)
			{
				return 0.0f;
			}
		}
	}

	for (int i = 0; i < planes.extent.size(); i++)
	{
		index<1> idx(i);
		rt_plane& pln = planes(idx);
		if (pln.get_resource_index() != exceptGeomIndex)
		{
			if (pln.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() >0)
			{
				return 0.0f;
			}
		}
	}

	for (int i = 0; i < cylinders.extent.size(); i++)
	{
		index<1> idx(i);
		rt_cylinder& cyl = cylinders(idx);
		if (cyl.get_resource_index() != exceptGeomIndex)
		{
			if (cyl.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() > 0)
			{
				return 0.0f;
			}
		}
	}
	return 1.0f;
}