#include "rt_gateway.h"
#include <chrono>
#include "EasyBMP.h"



scene_results rt_gateway::ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles,
	vector<rt_triangle> triangles, vector<rt_plane> planes, vector<rt_cylinder> cylinders,
	vector<rt_material> materials, vector<rt_directional_light> d_lights,
	vector<rt_point_light> p_lights, vector<rt_spot_light> s_lights,
	vector<rt_area_light> a_lights, float_3 ambience_color, float ambience_intensity,
	rt_camera camera, image_spec spec, map<int, tuple<string, int, int>> _vec_bmps, vector<float_3> scalars,
	map<int, tuple<string, int, int>> _flt_bmps, vector<float> f_scalars, int max_bmp_width, int max_bmp_height)
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
	vector<float_3> bitmaps(max_bmp_width * max_bmp_height * _vec_bmps.size());
	vector<float> f_bitmaps(max_bmp_width * max_bmp_height * _flt_bmps.size());
	load_vec_bmps(bitmaps, _vec_bmps, max_bmp_width, max_bmp_height);
	load_flt_bmps(f_bitmaps, _flt_bmps, max_bmp_width, max_bmp_height);

	texture<float_3, 3> bitmaps_txt(max_bmp_width, max_bmp_height, _vec_bmps.size(), bitmaps.begin(),bitmaps.end());
	texture<float_3, 1> scalars_txt(scalars.size(),scalars.begin(),scalars.end());

	texture<float, 3> f_bitmaps_txt(max_bmp_width, max_bmp_height, _flt_bmps.size(), f_bitmaps.begin(),f_bitmaps.end());
	texture<float, 1> f_scalars_txt(f_scalars.size(),f_scalars.begin(),f_scalars.end());

	texture_view<float_3, 3> bitmaps_view(bitmaps_txt);
	texture_view<float_3, 1> scalars_view(scalars_txt);
	texture_view<float_3,


	rt_core ray_tracer = rt_core(camera, spec, static_cast<int>(time(NULL)), spec.get_samples_per_pixel(), ambience_color, ambience_intensity);
	bool tile = true;
	if (tile)
	{
		const int tile_x = 8;
		const int tile_y = 8;
		parallel_for_each(image_view.extent.tile<tile_x, tile_y>(), [=](tiled_index<tile_x, tile_y> t_idx) mutable restrict(amp) {

			pixel_data data = ray_tracer.compute_pixel_data(t_idx.global[0], t_idx.global[1], sphere_view, rectangle_view, triangle_view,
				plane_view, cylinder_view, d_lights_view, p_lights_view, a_lights_view, s_lights_view, materials_view, bitmaps_view, scalars_view, f_bitmaps_view, f_scalars_view);
			image_view[t_idx] = data.get_pixel_color();
			coverage_mask_view[t_idx] = data.get_pixel_coverage();
			depth_map_view[t_idx] = data.get_pixel_depth();

		});
	}
	else {
		parallel_for_each(image_view.extent, [=](index<2> idx) mutable restrict(amp) {
			pixel_data data = ray_tracer.compute_pixel_data(idx[0], idx[1], sphere_view, rectangle_view,
				triangle_view, plane_view, cylinder_view, d_lights_view, p_lights_view, a_lights_view, s_lights_view, materials_view, bitmaps_view, scalars_view, f_bitmaps_view, f_scalars_view);
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



void rt_gateway::load_vec_bmps(vector<float_3>& buffer, map<int, tuple<string, int, int>>& bmps, int max_width, int max_height)
{
	int texel_index = 0;
	int read_texel_index = 0;
	int bmp_index = 0;
	int _max_texel_count = max_width * max_height * bmps.size();
	for (auto itr = bmps.begin(); itr != bmps.end(); itr++)
	{
		texel_index += (_max_texel_count - read_texel_index);
		read_texel_index = 0;
		string file = get<0>(itr->second);
		int width = get<1>(itr->second);
		int height = get<2>(itr->second);

		BMP bmp_file;
		bmp_file.ReadFromFile(file.c_str());



#pragma omp parallel for
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float_3 texel;
				auto pixel = bmp_file(x, y);
				texel.r = static_cast<float>(pixel->Red) / 255;
				texel.g = static_cast<float>(pixel->Green) / 255;
				texel.b = static_cast<float>(pixel->Blue) / 255;
				buffer[texel_index++] = texel;


			}
			read_texel_index++;
		}
	}

}


void rt_gateway::load_flt_bmps(vector<float>& buffer, map<int, tuple<string, int, int>>& bmps, int max_width, int max_height)
{
	int texel_index = 0;
	int read_texel_index = 0;
	int bmp_index = 0;
	int _max_texel_count = max_width * max_height * bmps.size();
	for (auto itr = bmps.begin(); itr != bmps.end(); itr++)
	{
		texel_index += (_max_texel_count - read_texel_index);
		read_texel_index = 0;
		string file = get<0>(itr->second);
		int width = get<1>(itr->second);
		int height = get<2>(itr->second);

		BMP bmp_file;
		bmp_file.ReadFromFile(file.c_str());



#pragma omp parallel for
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float texel;
				auto pixel = bmp_file(x, y);
				texel = (static_cast<float>(pixel->Red) + static_cast<float>(pixel->Green) +
					static_cast<float>(pixel->Blue)) / 765;
				buffer[texel_index++] = texel;


			}
			read_texel_index++;
		}
	}

}