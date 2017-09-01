#include "pixel_data.h"

using namespace rt;

pixel_data::pixel_data()
{

}

pixel_data& pixel_data::operator=(const pixel_data& rhs) 
{
	this->m_color = rhs.get_color();
	this->m_coverage = rhs.get_coverage();
	this->m_depth = rhs.get_depth();
	return *this;
}

pixel_data::pixel_data(vector<float> color, float coverage, float depth)
{
	m_color = color;
	m_coverage = coverage;
	m_depth = depth;
}

void pixel_data::set_color(vector<float> color) 
{
	m_color = color;
}

void pixel_data::set_coverage(float coverage)
{
	m_coverage = coverage;
}
void pixel_data::set_depth(float depth)
{
	m_depth = depth;
}

vector<float> pixel_data::get_color() const
{
	return m_color;
}

float pixel_data::get_coverage() const
{
	return m_coverage;
}

float pixel_data::get_depth() const
{
	return m_depth;
}

vector<float> pixel_data::get_default_color()
{
	return {0,0,0};
}