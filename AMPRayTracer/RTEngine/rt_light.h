#pragma once
#include <amp.h>
#include <amp_graphics.h>

#include "rt_sphere.h"
#include "rt_rectangle.h"

using namespace concurrency;
using namespace concurrency::graphics;
using namespace rt_support::geometries;

namespace rt_support
{
	namespace lights
	{


		struct light_type
		{
			static const int DIRECTIONAL = 1;
			static const int POINT = 2;
			static const int SPOT = 3;
			static const int AREA = 4;
		};

		class rt_light
		{
		protected:
			int m_type;
			float_3 m_color;
			float_3 m_direction; //does not apply to point light
			float_3 m_origin; //does not apply to directional light
			float m_range; //farthest distance light can travel (does not apply to directional light)
			float m_att_frac; //fraction of the range where attenuation starts
			int m_realistic_att; // a flag to determine if attenuation was linear or cosine based (realistic)
			float compute_attenuation(float hitDist) restrict(amp);

		public:
			__declspec(dllexport) rt_light(int type) restrict(amp,cpu);

			void set_color(float_3 color) restrict(amp,cpu);
			void set_direction(float_3 direction) restrict(amp, cpu);
			void set_origin(float_3 origin) restrict(amp, cpu);
			void set_range(float range) restrict(amp, cpu);
			void set_att_frac(float att_frac) restrict(amp, cpu);
			void set_realistic_att(int realistic_att) restrict(amp, cpu);

			float_3 get_color() const restrict(amp);
			float_3 get_direction() const restrict(amp);
			float_3 get_origin() const restrict(amp);
			float get_range() const  restrict(amp);
			float get_att_frac() const restrict(amp);
			int get_is_realistic_att() const restrict(amp);

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray (GPU hates virtual functions)
			///</summary>
			float percent_light(array_view<rt_rectangle,1>& rects, array_view<rt_sphere,1>& spheres, float_3 geomPoint, int exceptGeomIndex) restrict(amp);


		};



	}
}