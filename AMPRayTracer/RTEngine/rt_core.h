#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include "pixel_data.h"
#include "rt_camera.h"
#include "amp_lcg_rand.h"
#include "ray.h"
#include "intersection_record.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "rt_shader.h"
#include "rt_visibility.h"

#define INVALID_INDEX -1
#define FAR_PLANE_DIST 20


using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
namespace rt
{

	///<summary>
	///The core of the ray-tracer that initiates computation per pixel
	///</summary>
	class rt_core
	{
	private:
		rt_camera m_camera;
		amp_lcg_rand m_rand;
		int m_num_of_samples;
		rt_shader m_shader;
		rt_visibility m_visibility;
		
		
	public:
		
		

		//seed used for random number generation
		rt_core(rt_camera camera, image_spec spec, int seed, int no_of_samples, float_3 ambient_light, float ambient_intensity ) restrict(amp,cpu);
		
		// computes pixel data given its x and y offsets from top left pixel
		// also takes number of samples to take as a parameter
		pixel_data compute_pixel_data(int current_x, int current_y, array_view<rt_sphere, 1> spheres,array_view<rt_rectangle,1> rectangles,
			array_view<rt_triangle, 1> triangles, array_view<rt_plane, 1> planes, array_view<rt_cylinder, 1> cylinders,
			array_view<rt_directional_light, 1> dir_lights, array_view<rt_point_light, 1> point_lights, array_view<rt_area_light, 1> area_lights,
			array_view<rt_spot_light, 1> spot_lights, array_view<rt_material, 1> materials,  texture<float_3, 3> bitmaps, texture<float_3, 1> scalars
			, texture<float, 3> f_bitmaps, texture<float, 1> f_scalars) restrict(amp);

		
	};
}