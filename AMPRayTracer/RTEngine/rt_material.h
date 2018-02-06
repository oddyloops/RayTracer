#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include "indexed_resource.h"
#include "texture_map.h"

using namespace concurrency;
using namespace concurrency::graphics;

namespace rt_support
{
	namespace scene_resource_support
	{
		class rt_material : public indexed_resource
		{
		private:
			texture_map<float_3> m_ambient_color;
			texture_map<float_3> m_diffuse_color;
			texture_map<float_3> m_specular_color;
			texture_map<float> m_specularity;
			texture_map<float> m_refractive_index;
			texture_map<float> m_transparency;
			texture_map<float> m_reflectivity;
		public:
			__declspec(dllexport) rt_material() restrict(amp,cpu);

			__declspec(dllexport) rt_material(texture_map<float_3> ambient_color, texture_map<float_3> diffuse_color) restrict(amp,cpu);

			__declspec(dllexport) rt_material(texture_map<float_3> ambient_color, texture_map<float_3> diffuse_color, texture_map<float_3> specular_color, texture_map<float> specularity) restrict(amp,cpu);

			__declspec(dllexport) void set_ref_properties(texture_map<float> ref_index, texture_map<float> transparency, texture_map<float> reflectivity)  restrict(amp, cpu);

			void set_diffuse(texture_map<float_3> color) restrict(amp,cpu);
			
			void set_ambience(texture_map<float_3>  color) restrict(amp, cpu);

			void set_specular(texture_map<float_3>  color) restrict(amp, cpu);

			void set_specular(texture_map<float_3>  color, texture_map<float>  specularity)  restrict(amp, cpu);

			float_3 get_ambient_color(float u, float v, texture_view<const float_3,3> bitmaps, texture_view<const float_3,1> scalars) restrict(amp);

			float_3 get_diffuse_color(float u, float v, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars) restrict(amp);
			
			float_3 get_specular_color(float u, float v, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars) restrict(amp);

			float get_specularity(float u, float v, texture_view<const float, 3> bitmaps, texture_view<const float, 1> scalars) restrict(amp);

			float get_refractive_index(float u, float v, texture_view<const float, 3> bitmaps, texture_view<const float, 1> scalars) restrict(amp);

			float get_transparency(float u, float v, texture_view<const float, 3> bitmaps, texture_view<const float, 1> scalars) restrict(amp);

			float get_reflectivity(float u, float v, texture_view<const float, 3> bitmaps, texture_view<const float, 1> scalars) restrict(amp);

			int get_is_specular() restrict(amp);

			int get_is_reflective_refractive() restrict(amp);
			
		};
	}
}