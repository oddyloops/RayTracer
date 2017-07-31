#include "rt_gateway.h"
#include <chrono>

scene_results rt_gateway::ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles, vector<rt_material> materials, rt_camera camera,image_spec spec)
{
	camera.set_image_spec(spec);
	camera.compute_near_plane();
	int no_of_pixels = camera.get_image_spec().get_x_resolution() * camera.get_image_spec().get_y_resolution();
	scene_results results = { vector<float_3>(no_of_pixels),vector<float_3>(no_of_pixels) ,vector<float_3>(no_of_pixels) };
	
	auto now = std::chrono::system_clock::now();

	array_view<rt_rectangle, 1> rectangle_view(rectangles);
	array_view<rt_sphere, 1> sphere_view(spheres);
	
	array_view<float_3, 2> image_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), results[0]);
	array_view<float_3, 2> coverage_mask_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), results[1]);
	array_view<float_3, 2> depth_map_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), results[2]);


	rt_core ray_tracer = rt_core(camera, static_cast<int>(time(NULL)),spec.get_samples_per_pixel());

	parallel_for_each(image_view.extent, [=](index<2> idx) mutable restrict(amp)  {
		pixel_data data = ray_tracer.compute_pixel_data(idx[0], idx[1], sphere_view, rectangle_view);
		image_view[idx] = data.get_pixel_color();
		coverage_mask_view[idx] = data.get_pixel_coverage();
		depth_map_view[idx] = data.get_pixel_depth();
	});

	image_view.synchronize();
	coverage_mask_view.synchronize();
	depth_map_view.synchronize();
	
	auto duration = std::chrono::system_clock::now() - now;
	printf("Runtime: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
	
	return results;
}