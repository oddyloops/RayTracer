#include "image_spec.h"

using namespace rt_support;

image_spec::image_spec()
{
	m_x_resolution = 64;
	m_y_resolution = 64;
	m_samples_per_pixel = 1;
}

image_spec::image_spec(int x_resolution,int y_resolution,int samples_per_pixel)
{
	m_x_resolution = x_resolution;
	m_y_resolution = y_resolution;
	m_samples_per_pixel = samples_per_pixel;
}


int image_spec::get_samples_per_pixel()
{
	return m_samples_per_pixel;
}

int image_spec::get_x_resolution()
{
	return m_x_resolution;
}

int image_spec::get_y_resolution()
{
	return m_y_resolution;
}

float image_spec::get_aspect_ratio()
{
	return (float)m_x_resolution / m_y_resolution;
}