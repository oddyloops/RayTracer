#pragma once
#include "rt_light.h"

namespace rt_support
{
	namespace lights
	{

		class rt_directional_light : public rt_light
		{
		public: 
			rt_directional_light(vector<float> direction);

			rt_directional_light(vector<float> direction,vector<float> color);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<float> geomPoint, int exceptGeomIndex);
		};

	}
}
