#include "all_map.h"
#include "vector_util.h"

using namespace rt_support::scene_resource_support;

all_map::all_map() : m_is_null(true), m_map_type(map_type::plain) {}


all_map::all_map(bool is_file_source, int map_type) : m_is_file_source(is_file_source),m_map_type(map_type), m_is_null(false)
{
	
}

void all_map::set_direction(vector<float> direction)
{
	m_direction = vector_util::normalize(direction);
}

void all_map::set_stripe_width_perc(float stripe_width_perc)
{
	m_stripe_width_perc = stripe_width_perc;
}

void all_map::set_checker_dim(float x, float y)
{
	m_x_no = x;
	m_y_no = y;
}

bool all_map::is_null() const
{
	return m_is_null;
}

all_map& all_map::operator=(const all_map& a)
{
	m_is_file_source = a.m_is_file_source;
	m_map_type = a.m_map_type;
	m_direction = a.m_direction;
	m_is_null = a.m_is_null;
	m_stripe_width_perc = a.m_stripe_width_perc;
	m_x_no = a.m_x_no;
	m_y_no = a.m_y_no;

	return *this;
}