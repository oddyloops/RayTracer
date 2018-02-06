#include "rt_point_light.h"
#include "math_util.h"
#include "vector_amp.h"
using namespace rt_support::lights;



///<summary>
///Determines the percentage of light a pixel gets based on intersection with the light ray
///</summary>
float rt_point_light::percent_light(array_view<rt_geometry, 1>& geom, float_3 geomPoint, int exceptGeomIndex, float_3& origin,  float& dist) restrict(amp)
{
	ray r(geomPoint, origin);
	intersection_record rec;

	dist = vector_amp::magnitude(origin - r.get_origin()); //distance between point light and geometry

	for (int i = 0; i < geom.extent.size(); i++)
	{
		index<1> idx(i);
		rt_geometry& g = geom(idx);
		if (g.get_resource_index() != exceptGeomIndex)
		{

			if (g.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
			{

				return 0.0f;
			}
		}
	}

	

	return 1.0f;
}