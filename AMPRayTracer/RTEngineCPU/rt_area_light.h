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
			float m_focus; //distance between actual origin and area light
			float m_padding; //distance between the borders of both rectangles
			vector<float> m_hor_direction; //the direction along the width of the light area
			vector<float> m_ver_direction;  //the direction along the height of the light area

			void intiliaze_rectangles(float vertical_tilt,float focus, float lit_width, float lit_height);

		public:
			rt_area_light(vector<float> origin, vector<float> direction,float vertical_tilt, float focus, float lit_width,float lit_height, float padding,  float drop_constant);

			rt_area_light(vector<float> origin, vector<float> direction, float vertical_tilt, float focus, float lit_width, float lit_height, float padding, float drop_constant, vector<float> color);

			rt_area_light(vector<float> origin, vector<float> direction, float vertical_tilt, float focus, float lit_width, float lit_height, float padding, float drop_constant, vector<float> color, float range, float att_frac, bool is_real_att);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<float> geomPoint, int exceptGeomIndex);

		};
	}
}