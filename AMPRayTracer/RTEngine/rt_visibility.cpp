#include "rt_visibility.h"

using namespace rt;

rt_visibility::rt_visibility() restrict(amp,cpu)
{

}

void rt_visibility::compute_visibility(ray r, int except_geom_index, intersection_record& rec, array_view<rt_sphere, 1>* spheres, array_view<rt_rectangle, 1>* rects) restrict(amp)
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
}
