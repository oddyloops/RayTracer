#include "rt_gateway.h"
#include <chrono>



scene_results rt_gateway::ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles, rt_camera camera,image_spec spec)
{
	camera.initialize_image(spec);
	int no_of_pixels = camera.get_image_spec().get_x_resolution() * camera.get_image_spec().get_y_resolution();
	scene_results results;
	results.color = vector<float_3>(no_of_pixels);
	results.coverage = vector<float>(no_of_pixels);
	results.depth = vector<float>(no_of_pixels);

	
	auto now = std::chrono::system_clock::now();

	array_view<rt_rectangle, 1> rectangle_view(rectangles);
	array_view<rt_sphere, 1> sphere_view(spheres);
	
	array_view<float_3, 2> image_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), results.color);
	array_view<float, 2> coverage_mask_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), results.coverage);
	array_view<float, 2> depth_map_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), results.depth);

	
	rt_core ray_tracer = rt_core(camera,spec, static_cast<int>(time(NULL)),spec.get_samples_per_pixel());
	bool tile = true;
	if (tile)
	{
		const int tile_x = 8;
		const int tile_y = 8;
		parallel_for_each(image_view.extent.tile<tile_x, tile_y>(), [=](tiled_index<tile_x, tile_y> t_idx) mutable restrict(amp) {
			
					pixel_data data = ray_tracer.compute_pixel_data(t_idx.global[0], t_idx.global[1], sphere_view, rectangle_view);
					image_view[t_idx] = data.get_pixel_color();
					coverage_mask_view[t_idx] = data.get_pixel_coverage();
					depth_map_view[t_idx] = data.get_pixel_depth();
				
		});
	}
	else {
		parallel_for_each(image_view.extent, [=](index<2> idx) mutable restrict(amp) {
			pixel_data data = ray_tracer.compute_pixel_data(idx[0], idx[1], sphere_view, rectangle_view);
			image_view[idx] = data.get_pixel_color();
			coverage_mask_view[idx] = data.get_pixel_coverage();
			depth_map_view[idx] = data.get_pixel_depth();

		});
	}

	image_view.synchronize();
	coverage_mask_view.synchronize();
	depth_map_view.synchronize();
	
	auto duration = std::chrono::system_clock::now() - now;
	printf("Runtime: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
	
	return results;
}