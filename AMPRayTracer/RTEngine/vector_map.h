#pragma once
#include "texture_map.h"


using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::graphics;


namespace rt_support
{
	namespace scene_resource_support
	{



		class vector_map : public texture_map
		{

			float_3 get_value_plain(float u, float v, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalar_colors)  restrict(amp);

			float_3 get_value_checkered(float u, float v, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalar_colors)  restrict(amp);

			float_3 get_value_stripes(float u, float v, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalar_colors)  restrict(amp);

			float_3 get_value_wavy(float u, float v, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalar_colors)  restrict(amp);
		public:


			__declspec(dllexport) vector_map() restrict(amp, cpu);


			__declspec(dllexport) vector_map(int m_is_bitmap, int map_type) restrict(amp, cpu);




			float_3 get_value(float u, float v, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalar_colors) restrict(amp); //get value based on supplied u-v coordinates

			static float_3 get_bitmap_color(int map_index, int width, int height, int hor_offset, int ver_offset, float u, float v, texture<float_3, 3>* bitmaps) restrict(amp);

			static float_3 get_scalar_color(int color_offset, int color_index, texture<float_3, 1>* scalar_colors) restrict(amp);


		};
	}
}