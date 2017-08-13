#pragma once
#include "rt_light.h"

namespace rt_support
{
	namespace lights
	{
		class rt_point_light : public rt_light
		{
		public:
			rt_point_light(vector<float> origin);

			rt_point_light(vector<float> origin, vector<float> color);

			rt_point_light(vector<float> origin, vector<float> color, float range,float att_frac,bool is_real_att);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<float> geomPoint, int exceptGeomIndex);
		};
	}
}