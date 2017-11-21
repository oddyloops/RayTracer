#include "rt_gateway.h"
#include <chrono>



scene_results rt_gateway::ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles,
	vector<rt_triangle> triangles, vector<rt_plane> planes, vector<rt_cylinder> cylinders,
	vector<rt_material> materials, vector<rt_directional_light> d_lights,
	vector<rt_point_light> p_lights, vector<rt_spot_light> s_lights,
	vector<rt_area_light> a_lights, float_3 ambience_color, float ambience_intensity,
	rt_camera camera, image_spec spec, vector<float_3>* bitmaps, int no_of_bitmaps, vector<float_3> scalars,
	vector<float>* f_bitmaps, int no_of_f_bitmaps, vector<float> f_scalars, int max_bmp_width, int max_bmp_height)
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
	array_view<rt_triangle, 1> triangle_view(triangles);
	array_view<rt_plane, 1> plane_view(planes);
	array_view<rt_cylinder, 1> cylinder_view(cylinders);
	array_view<rt_directional_light, 1> d_lights_view(d_lights);
	array_view<rt_point_light, 1> p_lights_view(p_lights);
	array_view<rt_spot_light, 1> s_lights_view(s_lights);
	array_view<rt_area_light, 1> a_lights_view(a_lights);
	array_view<rt_material, 1> materials_view(materials);
	array_view<float_3, 2> image_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), results.color);
	array_view<float, 2> coverage_mask_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), results.coverage);
	array_view<float, 2> depth_map_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), results.depth);

	array_view<float_3, 3> bitmaps_view(max_bmp_width, max_bmp_height, no_of_bitmaps, *bitmaps);
	array_view<float_3, 1> scalars_view(scalars);

	array_view<float, 3> f_bitmaps_view(max_bmp_width, max_bmp_height, no_of_f_bitmaps, *f_bitmaps);
	array_view<float, 1> f_scalars_view(f_scalars);

	
	rt_core ray_tracer = rt_core(camera,spec, static_cast<int>(time(NULL)),spec.get_samples_per_pixel(),ambience_color,ambience_intensity);
	bool tile = true;
	if (tile)
	{
		const int tile_x = 8;
		const int tile_y = 8;
		parallel_for_each(image_view.extent.tile<tile_x, tile_y>(), [=](tiled_index<tile_x, tile_y> t_idx) mutable restrict(amp) {
			
			pixel_data data = ray_tracer.compute_pixel_data(t_idx.global[0], t_idx.global[1], sphere_view, rectangle_view, triangle_view,
				plane_view,cylinder_view,d_lights_view, p_lights_view, a_lights_view, s_lights_view,materials_view,bitmaps_view,scalars_view,f_bitmaps_view,f_scalars_view);
					image_view[t_idx] = data.get_pixel_color();
					coverage_mask_view[t_idx] = data.get_pixel_coverage();
					depth_map_view[t_idx] = data.get_pixel_depth();
				
		});
	}
	else {
		parallel_for_each(image_view.extent, [=](index<2> idx) mutable restrict(amp) {
			pixel_data data = ray_tracer.compute_pixel_data(idx[0], idx[1], sphere_view, rectangle_view,
				triangle_view,plane_view, cylinder_view,d_lights_view, p_lights_view, a_lights_view, s_lights_view, materials_view, bitmaps_view, scalars_view, f_bitmaps_view, f_scalars_view);
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