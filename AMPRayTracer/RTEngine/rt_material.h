#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include "indexed_resource.h"

using namespace concurrency;
using namespace concurrency::graphics;

namespace rt_support
{
	namespace scene_resource_support
	{
		class rt_material : public indexed_resource
		{
		private:
			float_3 m_ambient_color;
			float_3 m_diffuse_color;
			float_3 m_specular_color;
			float m_specularity;
		public:
			__declspec(dllexport) rt_material();

			__declspec(dllexport) rt_material(float_3 ambient_color, float_3 diffuse_color) restrict(amp,cpu);

			__declspec(dllexport) rt_material(float_3 ambient_color, float_3 diffuse_color, float_3 specular_color, float specularity) restrict(amp,cpu);

			void set_diffuse(float_3 color) restrict(amp,cpu);
			
			void set_ambience(float_3 color) restrict(amp, cpu);

			void set_specular(float_3 color) restrict(amp, cpu);

			void set_specular(float_3 color, float specularity)  restrict(amp, cpu);

			float_3 get_ambient_color() const restrict(amp);

			float_3 get_diffuse_color() const restrict(amp);
			
			float_3 get_specular_color() const restrict(amp);

			float get_specularity() const  restrict(amp);
			
		};
	}
}