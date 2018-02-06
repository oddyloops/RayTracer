#include "rt_directional_light.h"
#include "vector_amp.h"

using namespace rt_support::lights;





float rt_directional_light::percent_light(array_view<rt_geometry, 1>& geom, float_3 geomPoint, int exceptGeomIndex, float_3& direction) restrict(amp)
{
	ray r = ray::create_ray_from_pt_dir(geomPoint, -direction);
	intersection_record rec;
	for (int i =0; i < geom.extent.size(); i++)
	{
		index<1> idx(i);
		rt_geometry& g = geom(idx);
		if (g.get_resource_index() != exceptGeomIndex)
		{
			if (g.intersect(r, rec) && rec.get_hit_distance() > 0)
			{
				return 0.0f;
			}
		}
	}

	return 1.0f;
}