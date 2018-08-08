#pragma once

#include "vector_util.h"
#include "rt_camera.h"

using namespace rt_support;


namespace rt
{
	class rt_image_frame_support
	{
	private:
		rt_camera& m_cam;
	public:
		rt_image_frame_support(rt_camera& cam);

		vector<float> get_top_left_pixel_position();
		vector<float> get_top_right_position();
		vector<float> get_bottom_left_position();
		vector<float> get_bottom_right_position();

	};
}
