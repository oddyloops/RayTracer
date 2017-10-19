#include "rt_point_light.h"
#include "math_util.h"
#include "vector_amp.h"
using namespace rt_support::lights;

rt_point_light::rt_point_light(float_3 origin) restrict(amp,cpu) : rt_light(light_type::POINT)
{
	m_origin = origin;
}

rt_point_light::rt_point_light(float_3 origin, float_3 color) restrict(amp, cpu) : rt_light(light_type::POINT)
{
	m_origin = origin;
	m_color = color;
}


rt_point_light::rt_point_light(float_3 origin, float_3 color, float range, float att_frac, int is_real_att)  restrict(amp, cpu) : rt_light(light_type::POINT)
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
float rt_point_light::percent_light(array_view<rt_rectangle,1>& rects, array_view<rt_sphere,1>& spheres,
	array_view<rt_triangle, 1>& triangles, array_view<rt_plane, 1>& planes, array_view<rt_cylinder>& cylinders, float_3 geomPoint, int exceptGeomIndex) restrict(amp)
{
	ray r(geomPoint, m_origin);
	intersection_record rec;

	float dist = vector_amp::magnitude(m_origin - r.get_origin()); //distance between point light and geometry

	for (int i = 0; i < rects.extent.size(); i++)
	{
		index<1> idx(i);
		rt_rectangle& rect = rects(idx);
		if (rect.get_resource_index() != exceptGeomIndex)
		{

			if (rect.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
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
			if (sph.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
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
			if (tri.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
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
			if (pln.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
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
			if (cyl.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
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