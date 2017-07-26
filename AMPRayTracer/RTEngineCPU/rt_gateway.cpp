#include "rt_gateway.h"
#include "scene_database.h"
#include <chrono>

using namespace rt_support;

scene_results rt_gateway::ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles,  rt_camera camera,image_spec spec)
{
	
	int no_of_pixels = camera.get_image_spec().get_x_resolution() * camera.get_image_spec().get_y_resolution();
	scene_results results;
	
	auto now = std::chrono::system_clock::now();

	scene_database db = scene_database(spheres, rectangles);
	
	
	vector<vector<float>> image_view(no_of_pixels);
	vector<vector<float>> coverage_mask_view(no_of_pixels);
	vector<vector<float>> depth_map_view(no_of_pixels);
	


	rt_core ray_tracer = rt_core(camera, spec, static_cast<int>(time(NULL)),spec.get_samples_per_pixel(),db);


	for (int i = 0; i < spec.get_x_resolution(); i++)
	{
		for (int j = 0; j < spec.get_y_resolution(); j++)
		{
			pixel_data data = ray_tracer.compute_pixel_data(i, j);
			image_view[i * j] = data.get_color();
			coverage_mask_view[i*j] = { data.get_coverage() };
			depth_map_view[i*j] = { data.get_depth() };
		}
	}

	
	
	auto duration = std::chrono::system_clock::now() - now;
	printf("Runtime: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
	results.push_back(image_view);
	results.push_back(coverage_mask_view);
	results.push_back(depth_map_view);
	return results;
}