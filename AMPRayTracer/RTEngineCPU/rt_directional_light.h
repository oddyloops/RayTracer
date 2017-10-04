#pragma once
#include "rt_light.h"

namespace rt_support
{
	namespace lights
	{

		class rt_directional_light : public rt_light
		{
		public: 
			__declspec(dllexport) rt_directional_light(vector<float> direction);

			__declspec(dllexport) rt_directional_light(vector<float> direction,vector<float> color);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<rt_triangle>& triangles, vector<rt_plane>& planes, vector<float> geomPoint, int exceptGeomIndex);
		};

	}
}
