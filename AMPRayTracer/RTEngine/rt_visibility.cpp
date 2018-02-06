#include "rt_visibility.h"

using namespace rt;

rt_visibility::rt_visibility() restrict(amp,cpu)
{

}

void rt_visibility::compute_visibility(ray r, int except_geom_index, intersection_record& rec, array_view<rt_geometry, 1>* geom,texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
	, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp)
{
	for (int i = 0; i < geom->extent.size(); i++)
	{
		index<1> idx(i);
		if ((*geom)(idx).get_resource_index() != except_geom_index)
		{
			(*geom)(idx).intersect(r, rec,bitmaps,scalars,f_bitmaps,f_scalars);
		}
	}

	
}
