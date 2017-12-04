#include "texture_map.h"

using namespace rt_support::scene_resource_support;

texture_map::texture_map() restrict(amp, cpu)
{
	m_is_null = true;
}

texture_map::texture_map(int is_bitmap, int map_type) restrict(amp, cpu)
{
	m_is_null = false;
	m_is_bitmap = is_bitmap;
	m_map_type = map_type;
}

int texture_map::is_null() restrict(amp)
{
	return m_is_null;
}

void texture_map::set_direction(float_2 direction)
{
	m_direction = direction;
}

void texture_map::set_stripe_width_perc(float stripe_width_perc)
{
	m_stripe_width_perc = stripe_width_perc;
}

void texture_map::set_checker_dim(float x, float y)
{
	m_x_no = x;
	m_y_no = y;
}

void texture_map::set_bitmap_index(int map_index, int hor_offset, int ver_offset, int width, int height)
{
	m_map_index = map_index;
	m_hor_offset = hor_offset;
	m_ver_offset = ver_offset;
	m_width = width;
	m_height = height;
}

void texture_map::set_scalar_index(int color_offset, int colors_length)
{
	m_color_offset = color_offset;
	m_colors_length = colors_length;
}