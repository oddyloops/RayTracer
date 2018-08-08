#pragma once
#include "rt_light.h"
#include "rt_rectangle.h"

using namespace rt_support::geometries;

namespace rt_support
{
	namespace lights
	{
		class rt_area_light : public rt_light
		{
		private:
			rt_rectangle m_lit_area; //represents the area of the light that is fully lit
			rt_rectangle m_penumbra; //represents the surrounding area that is partly lit
			float m_drop_constant; //used fot computing penumbra attenuation (higher values means steeper attenuations)
		
			float m_padding; //distance between the borders of both rectangles
			vector<float> m_hor_direction; //the direction along the width of the light area
			vector<float> m_ver_direction;  //the direction along the height of the light area

			void initialize_penumbra();

		public:
			__declspec(dllexport) rt_area_light(vector<float> direction, rt_rectangle lit_area , float padding, float drop_constant);

			__declspec(dllexport) rt_area_light( vector<float> direction, rt_rectangle lit_area, float padding, float drop_constant, vector<float> color);

			__declspec(dllexport) rt_area_light(vector<float> direction, rt_rectangle lit_area, float padding, float drop_constant, vector<float> color, float range, float att_frac, bool is_real_att);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<rt_triangle>& triangles,  vector<rt_plane>& planes, vector<rt_cylinder>& cylinders,  vector<float> geomPoint, int exceptGeomIndex);

		};
	}
}