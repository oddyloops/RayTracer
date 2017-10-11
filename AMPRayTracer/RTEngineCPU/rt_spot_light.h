#pragma once
#include "rt_light.h"

namespace rt_support
{
	namespace lights
	{

		class rt_spot_light : public rt_light
		{
		private:
			float m_total_fov; //maximum coverage angle of the spot light in degrees
			float m_central_fov; //coverage angle without penumbra attenuation in degrees
			float m_cos_half_fov; //cosine of half the total fov angle
			float m_cos_half_cent_fov; //cosine of half the central fov angle
			float m_drop_constant; //used fot computing penumbra attenuation (higher values means steeper attenuations)
		public:
			__declspec(dllexport) rt_spot_light(vector<float> origin, vector<float> direction, float total_fov, float central_fov, float drop_constant);

			__declspec(dllexport) rt_spot_light(vector<float> origin, vector<float> direction, float total_fov, float central_fov, float drop_constant,vector<float> color);

			__declspec(dllexport) rt_spot_light(vector<float> origin, vector<float> direction, float total_fov, float central_fov, float drop_constant, vector<float> color, float range, float att_frac, bool is_real_att);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<rt_triangle>& triangles, vector<rt_plane>& planes, vector<rt_cylinder>& cylinders, vector<rt_cube>& cubes, vector<float> geomPoint, int exceptGeomIndex);

		};
	}
}
