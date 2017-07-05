#pragma once
#include "rt_visibility.h"
#include "rt_shader.h"
#include "rt_camera.h"


#define INVALID_INDEX -1
#define FAR_PLANE_DIST 40


using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
namespace rt
{

	class rt_core
	{
	private:
		rt_camera m_camera;
		int m_num_of_samples;
		rt_shader m_shader;
		rt_visibility m_visibility;

		
		// computes pixel data given the top-left corner position of the current pixel
		// also takes number of samples to take as a parameter (uses random average super-sampling)
		pixel_data compute_pixel_data_with_samples(vector<float> position, vector<rt_sphere> spheres, vector<rt_rectangle> rectangles);


		// compute the pixel data of the with given position
		pixel_data compute_sample_pixel_data(vector<float> sample_position, vector<rt_sphere> spheres, vector<rt_rectangle> rectangles);

		
	public:
		
		

		//seed used for random number generation
		rt_core(rt_camera camera,int seed, int no_of_samples);
		
		// computes pixel data given its x and y offsets from top left pixel
		// also takes number of samples to take as a parameter
		pixel_data compute_pixel_data(int current_x, int current_y,vector<rt_sphere> spheres, vector<rt_rectangle> rectangles);

		
	};
}