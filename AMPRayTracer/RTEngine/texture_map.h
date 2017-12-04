#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include "vector_amp.h"
#include <amp_math.h>
namespace rt_support
{
	namespace scene_resource_support
	{
		struct map_type
		{
			static const int plain = 1;
			static const int stripes = 2;
			static const int checkered = 3;;
			static const int wavy = 4;
		};


		class texture_map
		{
		protected:
			int m_is_null;
			int m_is_bitmap;
			int m_map_type;

			//for scalars
			int m_color_offset;
			int m_colors_length;

			//for bitmaps
			int m_map_index;
			int m_width;
			int m_height;
			int m_hor_offset;
			int m_ver_offset;


			//variables required for different mapping types
			float_2 m_direction;// stripes, and waves (2D)
			float m_stripe_width_perc; //stripes and waves
			float m_x_no, m_y_no; //checkers dimensions
		public:
			texture_map() restrict(amp,cpu);

			texture_map(int is_bitmap, int map_type) restrict(amp, cpu);

			int is_null() restrict(amp);

			__declspec(dllexport) void set_direction(float_2 direction);

			__declspec(dllexport) void set_stripe_width_perc(float stripe_width_perc);

			__declspec(dllexport) void set_checker_dim(float x, float y);

			__declspec(dllexport) void set_bitmap_index(int map_index, int hor_offset, int ver_offset, int width, int height);

			__declspec(dllexport) void set_scalar_index(int color_offset, int colors_length);
		};
	}
}
