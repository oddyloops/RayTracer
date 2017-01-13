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

#define INVALID_INDEX -1
#define FAR_PLANE_DIST 40

using namespace concurrency;
using namespace concurrency::graphics;
using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
namespace rt
{

	class rt_core
	{
	private:
		rt_camera m_camera;
		amp_lcg_rand m_rand;
		int m_num_of_samples;
		
		// computes pixel data given the top-left corner position of the current pixel
		// also takes number of samples to take as a parameter (uses random average super-sampling)
		pixel_data compute_pixel_data_with_samples(float_3 position, array_view<rt_sphere, 1> spheres, array_view<rt_rectangle, 1> rectangles) restrict(amp);


		// compute the pixel data of the with given position
		pixel_data compute_sample_pixel_data(float_3 sample_position, array_view<rt_sphere, 1> spheres, array_view<rt_rectangle, 1> rectangles) restrict(amp);

		//no virtual inheritance so we have to separate all geometries to call their appropriate intersect methods
		void compute_visibility(ray r, int except_geom_index, intersection_record& rec, array_view<rt_sphere, 1> spheres, array_view<rt_rectangle, 1> rectangles) restrict(amp);

		float_3 compute_shade(intersection_record rec, int generation) restrict(amp);
	public:
		
		

		//seed used for random number generation
		rt_core(rt_camera camera,int seed, int no_of_samples) restrict(amp,cpu);
		
		// computes pixel data given its x and y offsets from top left pixel
		// also takes number of samples to take as a parameter
		pixel_data compute_pixel_data(int current_x, int current_y, array_view<rt_sphere, 1> spheres, array_view<rt_rectangle, 1> rectangles) restrict(amp);

		
	};
}