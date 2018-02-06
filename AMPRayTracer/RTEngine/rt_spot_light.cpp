#include "rt_spot_light.h"
#include "math_util.h"
#include "vector_amp.h"

using namespace rt_support::lights;




float rt_spot_light::percent_light(array_view<rt_geometry, 1>& geom, float_3 geomPoint, int exceptGeomIndex, float_3& origin, float_3& direction, float& half_cos_fov, float&
	half_cos_cent_fov, float& drop_constant, float& dist) restrict(amp)
{
	float percent = 1.0f;

	//determine if it is within the total fov
	ray r(geomPoint, origin);
	intersection_record rec;
	//compute cosine of angle between ray and centerline of the spotlight
	float cosine = vector_amp::dot(-r.get_direction(), direction);

	dist = vector_amp::magnitude(origin - r.get_origin()); //distance between spot light and geometry
	if (cosine > half_cos_fov)
	{
		//within the coverage of the spotlight

	
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


		
		//no blockage
	
		//is it outside the central max intensity circle
		if (cosine < half_cos_cent_fov)
		{
			//compute spotlight penumbra attenuation
			percent *= powf((cosine - half_cos_fov) / (half_cos_cent_fov - half_cos_fov), drop_constant);
		}

		return percent;
	}
	return 0.0f;
}