#pragma once
#include "rt_light.h"

namespace rt_support
{
	namespace lights
	{
		class rt_point_light : public rt_light
		{
		public:
			rt_point_light(float_3 origin) restrict(amp,cpu);

			rt_point_light(float_3 origin, float_3 color) restrict(amp, cpu);

			rt_point_light(float_3 origin, float_3 color, float range, float att_frac, int is_real_att) restrict(amp, cpu);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(array_view<rt_rectangle,1>& rects, array_view<rt_sphere,1>& spheres, float_3 geomPoint, int exceptGeomIndex) restrict(amp);
		};
	}
}
