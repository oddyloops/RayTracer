#pragma once
#include "rt_light.h"

namespace rt_support
{
	namespace lights
	{
		class rt_point_light : public rt_light
		{
		public:
			__declspec(dllexport) rt_point_light(vector<float> origin);

			__declspec(dllexport) rt_point_light(vector<float> origin, vector<float> color);

			__declspec(dllexport) rt_point_light(vector<float> origin, vector<float> color, float range,float att_frac,bool is_real_att);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<rt_triangle>& triangles, vector<rt_plane>& planes, vector<rt_cylinder>& cylinders, vector<float> geomPoint, int exceptGeomIndex);
		};
	}
}