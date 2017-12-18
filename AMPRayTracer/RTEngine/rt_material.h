#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include "indexed_resource.h"
#include "vector_map.h"
#include "float_map.h"

using namespace concurrency;
using namespace concurrency::graphics;

namespace rt_support
{
	namespace scene_resource_support
	{
		class rt_material : public indexed_resource
		{
		private:
			vector_map m_ambient_color;
			vector_map m_diffuse_color;
			vector_map m_specular_color;
			float_map m_specularity;
			float_map m_refractive_index;
			float_map m_transparency;
			float_map m_reflectivity;
		public:
			__declspec(dllexport) rt_material() restrict(amp,cpu);

			__declspec(dllexport) rt_material(vector_map ambient_color, vector_map diffuse_color) restrict(amp,cpu);

			__declspec(dllexport) rt_material(vector_map ambient_color, vector_map diffuse_color, vector_map specular_color, float_map specularity) restrict(amp,cpu);

			__declspec(dllexport) void set_ref_properties(float_map ref_index, float_map transparency, float_map reflectivity)  restrict(amp, cpu);

			void set_diffuse(vector_map color) restrict(amp,cpu);
			
			void set_ambience(vector_map  color) restrict(amp, cpu);

			void set_specular(vector_map  color) restrict(amp, cpu);

			void set_specular(vector_map  color, float_map  specularity)  restrict(amp, cpu);

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