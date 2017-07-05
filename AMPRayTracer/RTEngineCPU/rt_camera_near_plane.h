#pragma once

#include "orthonormal_basis.h"
#include "image_spec.h"

// helper calss that holds info about the 
// camera's near plane

namespace rt_support
{
	class rt_camera_near_plane
	{
	private:
		float m_width;               // near plane width
		float m_height;              // near plane height
		float m_pixel_width;          // pixel width
		float m_pixel_height;         // pixel height
		vector<float> m_center;            // center of the near plane

		vector<float> m_vertices[4];        // ul, ur, ll, lr
		const int UL = 0;          // Upper-Left corner
		const int UR = 1;          // Upper-Right corner
		const int LR = 2;          // Lower-Left corner
		const int LL = 3;          // Lower-Left corner

	public:
		
		rt_camera_near_plane();

		// public constructor that creates RTCameraNearPlane from
		// eye position, the focus of camera, the vertical fov, 
		// orthonormal basis, and image spec 
		rt_camera_near_plane(vector<float> eye, float focus, float fov, orthonormal_basis basis, image_spec spec);

		rt_camera_near_plane& operator=(const rt_camera_near_plane& cam);

		float get_width() const;
		float get_height() const;
		float get_pixel_width() const;
		float get_pixel_height() const;
		vector<float> get_center() const;

		vector<float> get_upper_left() const;
		vector<float> get_lower_left() const;
		vector<float> get_upper_right() const;
		vector<float> get_lower_right() const;

	};
}
