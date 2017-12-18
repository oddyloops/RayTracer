#pragma once
#include "texture_map.h"


using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::graphics;


namespace rt_support
{
	namespace scene_resource_support
	{
	

		
		class float_map : public texture_map
		{

			float get_value_plain(float u, float v, texture_view<const float, 3> bitmaps, texture_view<const float, 1> scalar_colors)  restrict(amp);

			float get_value_checkered(float u, float v, texture_view<const float, 3> bitmaps, texture_view<const float, 1> scalar_colors)  restrict(amp);

			float get_value_stripes(float u, float v, texture_view<const float, 3> bitmaps, texture_view<const float, 1> scalar_colors)  restrict(amp);

			float get_value_wavy(float u, float v, texture_view<const float, 3> bitmaps, texture_view<const float, 1> scalar_colors)  restrict(amp);

			static float get_bitmap_color(int map_index, int width, int height, int hor_offset, int ver_offset, float u, float v, texture_view<const float, 3> bitmaps) restrict(amp);

			static float get_scalar_color(int color_offset, int color_index, texture_view<const float, 1> scalar_colors) restrict(amp);
		public:


			__declspec(dllexport) float_map() restrict(amp, cpu);


			__declspec(dllexport) float_map(int m_is_bitmap, int map_type) restrict(amp, cpu);


			

			float get_value(float u, float v, texture_view<const float, 3> bitmaps, texture_view<const float, 1> scalar_colors) restrict(amp); //get value based on supplied u-v coordinates

		


		};
		
	}
}
