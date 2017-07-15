#include "rt_image_frame_support.h"

using namespace rt;

rt_image_frame_support::rt_image_frame_support(rt_camera& cam):m_cam(cam)
{}


vector<float> rt_image_frame_support::get_top_left_pixel_position()
{
	return m_cam.get_pixel_position(0, 0);
}


vector<float> rt_image_frame_support::get_top_right_position()
{
	return m_cam.get_pixel_position(0, m_cam.get_image_spec().get_y_resolution() - 1);
}

vector<float> rt_image_frame_support::get_bottom_left_position()
{
	return m_cam.get_pixel_position(m_cam.get_image_spec().get_x_resolution() - 1, 0);
}

vector<float> rt_image_frame_support::get_bottom_right_position()
{
	return m_cam.get_pixel_position(m_cam.get_image_spec().get_x_resolution() - 1, m_cam.get_image_spec().get_y_resolution() - 1);
}
