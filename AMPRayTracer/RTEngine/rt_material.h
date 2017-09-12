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
			float_3 m_specular_color = float_3(-1,-1,-1);
			float m_specularity = -1;
			float m_refractive_index = -1;
			float m_transparency = -1;
			float m_reflectivity = -1;
		public:
			__declspec(dllexport) rt_material();

			__declspec(dllexport) rt_material(float_3 ambient_color, float_3 diffuse_color) restrict(amp,cpu);

			__declspec(dllexport) rt_material(float_3 ambient_color, float_3 diffuse_color, float_3 specular_color, float specularity) restrict(amp,cpu);

			__declspec(dllexport) void set_ref_properties(float ref_index, float transparency, float reflectivity)  restrict(amp, cpu);

			void set_diffuse(float_3 color) restrict(amp,cpu);
			
			void set_ambience(float_3 color) restrict(amp, cpu);

			void set_specular(float_3 color) restrict(amp, cpu);

			void set_specular(float_3 color, float specularity)  restrict(amp, cpu);

			float_3 get_ambient_color() const restrict(amp);

			float_3 get_diffuse_color() const restrict(amp);
			
			float_3 get_specular_color() const restrict(amp);

			float get_specularity() const  restrict(amp);

			float get_refractive_index() const restrict(amp);

			float get_transparency() const restrict(amp);

			float get_reflectivity() const restrict(amp);

			int get_is_specular() restrict(amp);

			int get_is_reflective_refractive() restrict(amp);
			
		};
	}
}