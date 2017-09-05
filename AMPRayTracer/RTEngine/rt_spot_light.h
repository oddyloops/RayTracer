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
			__declspec(dllexport) rt_spot_light(float_3 origin, float_3 direction, float total_fov, float central_fov, float drop_constant) restrict(amp,cpu);

			__declspec(dllexport) rt_spot_light(float_3 origin, float_3 direction, float total_fov, float central_fov, float drop_constant, float_3 color) restrict(amp, cpu);

			__declspec(dllexport) rt_spot_light(float_3 origin, float_3 direction, float total_fov, float central_fov, float drop_constant, float_3 color, float range, float att_frac, int is_real_att) restrict(amp, cpu);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			float percent_light(array_view<rt_rectangle,1>& rects, array_view<rt_sphere,1>& spheres, float_3 geomPoint, int exceptGeomIndex) restrict(amp);

		};
	}
}
#pragma once
