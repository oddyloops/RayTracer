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
			float_3 m_hor_direction; //the direction along the width of the light area
			float_3 m_ver_direction;  //the direction along the height of the light area

			void initialize_penumbra() restrict(amp,cpu);

		public:
			__declspec(dllexport) rt_area_light(float_3 direction, rt_rectangle lit_area, float padding, float drop_constant) restrict(amp,cpu);

			__declspec(dllexport) rt_area_light(float_3 direction, rt_rectangle lit_area, float padding, float drop_constant, float_3 color) restrict(amp, cpu);

			__declspec(dllexport) rt_area_light(float_3 direction, rt_rectangle lit_area, float padding, float drop_constant, float_3 color, float range, float att_frac, int is_real_att)  restrict(amp, cpu);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(array_view<rt_rectangle, 1>& rects, array_view<rt_sphere, 1>& spheres, float_3 geomPoint, int exceptGeomIndex) restrict(amp);

		};
	}
}