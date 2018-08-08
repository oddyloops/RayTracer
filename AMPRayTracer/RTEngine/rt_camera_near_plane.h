#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include "orthonormal_basis.h"
#include "image_spec.h"
#include <amp_math.h>

using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::graphics;
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
		float_3 m_center;            // center of the near plane

		float_3 m_vertices[4];        // ul, ur, ll, lr
		const int UL = 0;          // Upper-Left corner
		const int UR = 1;          // Upper-Right corner
		const int LR = 2;          // Lower-Left corner
		const int LL = 3;          // Lower-Left corner

	public:
		
		rt_camera_near_plane() restrict(amp, cpu) {/*default constructor*/}

		// public constructor that creates RTCameraNearPlane from
		// eye position, the focus of camera, the vertical fov, 
		// orthonormal basis, and image spec 
		rt_camera_near_plane(float_3 eye, float focus, float fov, orthonormal_basis basis, image_spec spec) restrict(amp,cpu);

		rt_camera_near_plane& operator=(const rt_camera_near_plane& cam) restrict(amp, cpu);

		float get_width() const restrict(amp, cpu);
		float get_height() const restrict(amp, cpu);
		float get_pixel_width() const restrict(amp, cpu);
		float get_pixel_height() const restrict(amp, cpu);
		float_3 get_center() const restrict(amp, cpu);

		float_3 get_upper_left() const restrict(amp, cpu);
		float_3 get_lower_left() const restrict(amp, cpu);
		float_3 get_upper_right() const restrict(amp, cpu);
		float_3 get_lower_right() const restrict(amp, cpu);

	};
}
