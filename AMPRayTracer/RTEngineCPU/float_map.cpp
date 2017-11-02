#include "float_map.h"
#include "vector_util.h"





using namespace rt_support::scene_resource_support;

float_map::float_map() : all_map()
{

}


float_map::float_map(bool is_file_source, int map_type) : all_map(is_file_source,map_type)
{

}

void float_map::set_bitmap_source(vector<matrix<float>> bitmap_source)
{
	m_bitmap_source = bitmap_source;
}

void float_map::set_scalar_source(vector<float> scalar_source)
{
	m_scalar_source = scalar_source;
}

float float_map::get_value(float u, float v) //get value based on supplied u-v coordinates
{
	switch (m_map_type)
	{
	case map_type::checkered:
		return get_value_checkered(u, v);
		break;
	case map_type::stripes:
		return get_value_stripes(u, v);
		break;
	case map_type::wavy:
		return get_value_wavy(u, v);
		break;
	default:
		return get_value_plain(u, v);
		break;
	}
}


float float_map::get_value_stripes(float u, float v)
{
	int stripe_index = 0;
	vector<float> uv = { u,v };
	float dir_index = vector_util::dot(uv, m_direction);

	stripe_index = static_cast<int>(dir_index / m_stripe_width_perc);
	if (m_is_file_source)
	{
		matrix<float>& bmp = m_bitmap_source[stripe_index % m_bitmap_source.size()];
		//compute horizontal and vertical indices in the bitmap
		//horizontal index  starting from bottom left hence the substraction
		vector<float> perp_direction = { m_direction[1], -m_direction[0] };
		float v_run = vector_util::dot(uv, perp_direction);
		int r = bmp.get_row_length() - 1 - static_cast<int>(v_run * bmp.get_row_length());
		//vertical index
		int c = static_cast<int>((u - (m_x_no * dir_index)) * bmp.get_col_length());
		return bmp.at(r, c);
	}
	else {
		return m_scalar_source[stripe_index % m_scalar_source.size()];
	}
}

float float_map::get_value_checkered(float u, float v)
{
	int tile_index = 0;
	int x_index = static_cast<int>(u / m_x_no);
	int y_index = static_cast<int>(v / m_y_no);

	int source_size = m_is_file_source ? m_bitmap_source.size() : m_scalar_source.size();

	int vert = y_index % source_size;
	int hor = x_index;

	tile_index = vert + hor;

	if (m_is_file_source)
	{
		matrix<float>& bmp = m_bitmap_source[tile_index % source_size];
		//compute horizontal and vertical indices in the bitmap
		//horizontal index starting from bottom left hence the substraction
		int r = bmp.get_row_length() - 1 - static_cast<int>((v - (m_y_no * y_index)) * bmp.get_row_length());
		//vertical index
		int c = static_cast<int>((u - (m_x_no * x_index)) * bmp.get_col_length());
		return bmp.at(r, c);
	}
	else {

		return m_scalar_source[tile_index % source_size];
	}
}

float float_map::get_value_plain(float u, float v)
{
	if (m_is_file_source)
	{
		int r = m_bitmap_source[0].get_row_length() - 1 - static_cast<int>(v * m_bitmap_source[0].get_row_length());
		int c = static_cast<int>(u * m_bitmap_source[0].get_col_length());
		return m_bitmap_source[0].at(r, c);
	}
	else {
		return m_scalar_source[0];
	}
}

float float_map::get_value_wavy(float u, float v)
{
	int stripe_index = 0;
	float color1_frac, color2_frac = 1;
	vector<float> uv = { u,v };
	float dir_index = vector_util::dot(uv, m_direction);
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


	if (m_is_file_source)
	{
		matrix<float>& bmp1 = m_bitmap_source[stripe_index % m_bitmap_source.size()];
		matrix<float>& bmp2 = m_bitmap_source[(stripe_index + 1) % m_bitmap_source.size()];
		//compute horizontal and vertical indices in the bitmap
		//horizontal index  starting from bottom left hence the substraction
		vector<float> perp_direction = { m_direction[1], -m_direction[0] };
		float v_run = vector_util::dot(uv, perp_direction);
		int r1 = bmp1.get_row_length() - 1 - static_cast<int>(v_run * bmp1.get_row_length());
		int r2 = bmp2.get_row_length() - 1 - static_cast<int>(v_run * bmp2.get_row_length());
		//vertical index
		int c1 = static_cast<int>((u - (m_x_no * dir_index)) * bmp1.get_col_length());
		int c2 = static_cast<int>((u - (m_x_no * dir_index)) * bmp2.get_col_length());
		return color1_frac * bmp1.at(r1, c1) + color2_frac * bmp1.at(r2, c2);
	}
	else {
		return color1_frac * m_scalar_source[stripe_index % m_scalar_source.size()] +
			color2_frac * m_scalar_source[(stripe_index + 1) % m_scalar_source.size()];
	}
}

float_map& float_map::operator=(const float_map& a)
{
	m_is_file_source = a.m_is_file_source;
	m_map_type = a.m_map_type;
	m_direction = a.m_direction;
	m_is_null = a.m_is_null;
	m_stripe_width_perc = a.m_stripe_width_perc;
	m_x_no = a.m_x_no;
	m_y_no = a.m_y_no;
	m_scalar_source = a.m_scalar_source;
	m_bitmap_source = a.m_bitmap_source;
	return *this;
}