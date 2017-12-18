#include "rt_gateway.h"
#include "scene_database.h"
#include <chrono>

using namespace rt_support;

scene_results rt_gateway::ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles, vector<rt_triangle> triangles, vector<rt_plane> planes,
	vector<rt_cylinder> cylinders,
	vector<rt_material> materials, vector<rt_directional_light> d_lights,
	vector<rt_point_light> p_lights, vector<rt_spot_light> s_lights,
	vector<rt_area_light> a_lights, vector<float> ambience_color, float ambience_intensity, rt_camera camera,image_spec spec)
{
	camera.initialize_image(spec);
	int no_of_pixels = camera.get_image_spec().get_x_resolution() * camera.get_image_spec().get_y_resolution();
	scene_results results;
	
	auto now = std::chrono::system_clock::now();

	scene_database db = scene_database(spheres, rectangles,triangles,planes,cylinders,materials,a_lights,s_lights,d_lights,p_lights);
	
	
	vector<vector<float>> image_view(no_of_pixels);
	vector<vector<float>> coverage_mask_view(no_of_pixels);
	vector<vector<float>> depth_map_view(no_of_pixels);
	


	rt_core ray_tracer = rt_core(camera, spec, static_cast<int>(time(NULL)),spec.get_samples_per_pixel(),db,ambience_color,ambience_intensity);
	int index = 0;


	for (int i = 0; i < spec.get_x_resolution(); i++)
	{
#pragma omp parallel for
		for (int j = 0; j < spec.get_y_resolution(); j++)
		{
			
			pixel_data data = ray_tracer.compute_pixel_data(i, j);
			
			image_view[index] = data.get_color();
			coverage_mask_view[index] = { data.get_coverage() };
			depth_map_view[index++] = { data.get_depth() };
		}
	}

	
	
	auto duration = std::chrono::system_clock::now() - now;
	printf("Runtime: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
	results.push_back(image_view);
	results.push_back(coverage_mask_view);
	results.push_back(depth_map_view);
	return results;
}