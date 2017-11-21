#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include "vector_amp.h"
#include <amp_math.h>

using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::graphics;


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

		template <typename T>
		class texture_map
		{
		private:
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



			T get_value_plain(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors)  restrict(amp);

			T get_value_checkered(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors)  restrict(amp);

			T get_value_stripes(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors)  restrict(amp);

			T get_value_wavy(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors)  restrict(amp);
		public:


			__declspec(dllexport) texture_map() restrict(amp, cpu);


			__declspec(dllexport) texture_map(int m_is_bitmap, int map_type) restrict(amp, cpu);


			__declspec(dllexport) void set_direction(float_2 direction);

			__declspec(dllexport) void set_stripe_width_perc(float stripe_width_perc);

			__declspec(dllexport) void set_checker_dim(float x, float y);

			int is_null() restrict(amp);


			__declspec(dllexport) void set_bitmap_index(int map_index,int hor_offset,int ver_offset, int width, int height);

			__declspec(dllexport) void set_scalar_index(int color_offset, int colors_length);

			T get_value(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors) restrict(amp); //get value based on supplied u-v coordinates

			static T get_bitmap_color(int map_index, int width, int height,int hor_offset, int ver_offset,  float u, float v, array_view<T, 3>* bitmaps) restrict(amp);

			static T get_scalar_color(int color_offset, int color_index, array_view<T, 1>* scalar_colors) restrict(amp);


		};



		template<typename T>
		texture_map<T>::texture_map()  restrict(amp, cpu)
		{
			m_is_null = true;
		}

		template<typename T>
		texture_map<T>::texture_map(int is_bitmap, int map_type)  restrict(amp, cpu)
		{
			m_is_null = false;
			m_is_bitmap = is_bitmap;
			m_map_type=map_type;
		
		}

		template<typename T>
		void texture_map<T>::set_direction(float_2 direction)
		{
			m_direction = direction;
		}

		template<typename T>
		void texture_map<T>::set_checker_dim(float x, float y)
		{
			m_x_no = x;
			m_y_no = y;
		}

		template<typename T>
		void texture_map<T>::set_stripe_width_perc(float width_perc)
		{
			m_stripe_width_perc = width_perc;
		}

		template<typename T>
		void texture_map<T>::set_bitmap_index(int map_index, int hor_offset, int ver_offset, int width, int height)
		{
			m_map_index = map_index;
			m_hor_offset = hor_offset;
			m_ver_offset = m_ver_offset;
			m_width = width;
			m_height = height;
		}

		template<typename T>
		void texture_map<T>::set_scalar_index(int color_offset, int colors_length)
		{
			m_color_offset = color_offset;
			m_colors_length = colors_length;
		}

		template<typename T>
		int texture_map<T>::is_null() restrict(amp)
		{
			return m_is_null;
		}

		template<typename T>
		T texture_map<T>::get_value(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors)  restrict(amp)
		{
			switch (m_map_type)
			{
			case map_type::plain:
				return get_value_plain(u, v, bitmaps, scalar_colors);
				break;
			case map_type::checkered:
				return get_value_checkered(u, v, bitmaps, scalar_colors);
				break;
			case map_type::stripes:
				return get_value_stripes(u, v, bitmaps, scalar_colors);
				break;
			case map_type::wavy:
				return get_value_wavy(u, v, bitmaps, scalar_colors);
				break;
			default:
				return get_value_plain(u, v, bitmaps, scalar_colors);
			}
		}

		template <typename T>
		T texture_map<T>::get_value_plain(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors)  restrict(amp)
		{
			if (m_is_bitmap)
			{
				return get_bitmap_color(m_map_index, m_width, m_height,m_hor_offset,m_ver_offset, u, v, bitmaps);
			}
			else {
				return get_scalar_color(m_color_offset, 0, scalar_colors);
			}
		}


		template <typename T>
		T texture_map<T>::get_value_checkered(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors)  restrict(amp)
		{
			if (m_is_bitmap)
			{ //it complicate things in the GPU model to have multiple bitmaps
				return get_value_plain(u, v, bitmaps, scalar_colors);
			}


			int tile_index = 0;
			int x_index = static_cast<int>(u / m_x_no);
			int y_index = static_cast<int>(v / m_y_no);

			int source_size = m_colors_length;

			int vert = y_index % source_size;
			int hor = x_index;
			tile_index = vert + hor;
			return  get_scalar_color(m_color_offset, tile_index % source_size, scalar_colors);

		}

		template <typename T>
		T texture_map<T>::get_value_stripes(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors)  restrict(amp)
		{
			if (m_is_bitmap)
			{ //it complicate things in the GPU model to have multiple bitmaps
				return get_value_plain(u, v, bitmaps, scalar_colors);
			}

			int stripe_index = 0;
			float_2 uv = float_2(u, v);
			float dir_index = vector_amp::dot(uv, m_direction);

			stripe_index = static_cast<int>(dir_index / m_stripe_width_perc);

			return get_scalar_color(m_color_offset, stripe_index % m_colors_length, scalar_colors);


		}


		template <typename T>
		T texture_map<T>::get_value_wavy(float u, float v, array_view<T, 3>* bitmaps, array_view<T, 1>* scalar_colors)  restrict(amp)
		{
			if (m_is_bitmap)
			{ //it complicate things in the GPU model to have multiple bitmaps
				return get_value_plain(u, v, bitmaps, scalar_colors);
			}
			const float PI = 3.14159F;
			int stripe_index = 0;
			float color1_frac, color2_frac = 1;
			float_2 uv = float_2(u, v);
			float dir_index = vector_amp::dot(uv, m_direction);
			float f = dir_index / m_stripe_width_perc;
			stripe_index = static_cast<int>(f);
			float ninety = 0.5F * PI;
			float theta = ((f - stripe_index) * ninety);
			if (theta > ninety)
			{
				theta = PI - theta;
			}
			color1_frac = cosf(theta);
			color2_frac = 1 - color1_frac;

			return color1_frac * get_scalar_color(m_color_offset, stripe_index % m_colors_length, scalar_colors) +
				color2_frac * get_scalar_color(m_color_offset, (stripe_index + 1) % m_colors_length, scalar_colors);



		}


		template <typename T>
		T texture_map<T>::get_bitmap_color(int map_index, int width, int height, int hor_offset, int ver_offset, float u, float v, array_view<T, 3>* bitmaps) restrict(amp)
		{
			int r = height - 1 - static_cast<int>(v * height);
			int c = static_cast<int>(u * width);
			index<3> idx(map_index, r + ver_offset, c + hor_offset);
			return (*bitmaps)(idx);
		}

		template <typename T>
		T texture_map<T>::get_scalar_color(int color_offset, int color_index, array_view<T, 1>* scalar_colors) restrict(amp)
		{
			index<1> idx(color_offset + color_index);
			return (*scalar_colors)(idx);
		}
	}
}
