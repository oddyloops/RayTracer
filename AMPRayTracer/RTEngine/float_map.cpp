#include "float_map.h"

using namespace rt_support::scene_resource_support;


float float_map::get_value_plain(float u, float v, texture<float, 3>* bitmaps, texture<float, 1>* scalar_colors)  restrict(amp)
{
	if (m_is_bitmap)
	{
		return get_bitmap_color(m_map_index, m_width, m_height, m_hor_offset, m_ver_offset, u, v, bitmaps);
	}
	else {
		return get_scalar_color(m_color_offset, 0, scalar_colors);
	}
}

float  float_map::get_value_checkered(float u, float v, texture<float, 3>* bitmaps, texture<float, 1>* scalar_colors)  restrict(amp)
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

float  float_map::get_value_stripes(float u, float v, texture<float, 3>* bitmaps, texture<float, 1>* scalar_colors)  restrict(amp)
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

float  float_map::get_value_wavy(float u, float v, texture<float, 3>* bitmaps, texture<float, 1>* scalar_colors)  restrict(amp)
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

float_map::float_map() restrict(amp, cpu) : texture_map()
{

}


float_map::float_map(int m_is_bitmap, int map_type) restrict(amp, cpu) : texture_map(m_is_bitmap,map_type)
{

}

float float_map::get_value(float u, float v, texture<float, 3>* bitmaps, texture<float, 1>* scalar_colors) restrict(amp) //get value based on supplied u-v coordinates
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

float float_map::get_bitmap_color(int map_index, int width, int height, int hor_offset, int ver_offset, float u, float v, texture<float, 3>* bitmaps) restrict(amp)
{
	int r = height - 1 - static_cast<int>(v * height);
	int c = static_cast<int>(u * width);
	index<3> idx(map_index, r + ver_offset, c + hor_offset);
	return (*bitmaps)(idx);
}

float float_map::get_scalar_color(int color_offset, int color_index, texture<float, 1>* scalar_colors) restrict(amp)
{
	index<1> idx(color_offset + color_index);
	return (*scalar_colors)(idx);
}