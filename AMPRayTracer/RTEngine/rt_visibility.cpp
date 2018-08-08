#include "rt_visibility.h"

using namespace rt;

rt_visibility::rt_visibility() restrict(amp,cpu)
{

}

void rt_visibility::compute_visibility(ray r, int except_geom_index, intersection_record& rec, array_view<rt_sphere, 1>* spheres, array_view<rt_rectangle, 1>* rects,
	array_view<rt_triangle, 1>* triangles, array_view<rt_plane, 1>* planes, array_view<rt_cylinder, 1>* cylinders) restrict(amp)
{
	for (int i = 0; i < spheres->extent.size(); i++)
	{
		index<1> idx(i);
		if ((*spheres)(idx).get_resource_index() != except_geom_index)
		{
			(*spheres)(idx).intersect(r, rec);
		}
	}

	for (int i = 0; i < rects->extent.size(); i++)
	{
		index<1> idx(i);
		if ((*rects)(idx).get_resource_index() != except_geom_index)
		{
			(*rects)(idx).intersect(r, rec);
		}
	}

	for (int i = 0; i < triangles->extent.size(); i++)
	{
		index<1> idx(i);
		if ((*triangles)(idx).get_resource_index() != except_geom_index)
		{
			(*triangles)(idx).intersect(r, rec);
		}
	}

	for (int i = 0; i < planes->extent.size(); i++)
	{
		index<1> idx(i);
		if ((*planes)(idx).get_resource_index() != except_geom_index)
		{
			(*planes)(idx).intersect(r, rec);
		}
	}

	for (int i = 0; i < cylinders->extent.size(); i++)
	{
		index<1> idx(i);
		if ((*cylinders)(idx).get_resource_index() != except_geom_index)
		{
			(*cylinders)(idx).intersect(r, rec);
		}
	}
}
