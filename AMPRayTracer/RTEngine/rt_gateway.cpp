#include "rt_gateway.h"

void rt_gateway::ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles, vector<rt_material> materials, rt_camera camera,int samples_per_pixel)
{
	//create blank spaces for pixels
	int no_of_pixels = camera.get_image_spec().get_x_resolution() * camera.get_image_spec().get_y_resolution();
	fill(m_image.begin(),m_image.begin() + no_of_pixels, float_3(0, 0, 0));
	fill(m_coverage_mask.begin(), m_coverage_mask.begin() + no_of_pixels, float_3(0, 0, 0));
	fill(m_depth_map.begin(), m_depth_map.begin() + no_of_pixels, float_3(0, 0, 0));

	array_view<rt_sphere, 1> sphere_view(spheres);
	array_view<rt_rectangle, 1> rectangle_view(rectangles);
	array_view<float_3, 2> image_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(),m_image);
	array_view<float_3, 2> coverage_mask_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), m_coverage_mask);
	array_view<float_3, 2> depth_map_view(camera.get_image_spec().get_x_resolution(), camera.get_image_spec().get_y_resolution(), m_depth_map);


	rt_core ray_tracer = rt_core(camera, static_cast<int>(time(NULL)),samples_per_pixel);

	parallel_for_each(image_view.extent, [=](index<2> idx) mutable restrict(amp)  {
		pixel_data data = ray_tracer.compute_pixel_data(idx[0], idx[1], sphere_view, rectangle_view);
		image_view[idx] = data.get_pixel_color();
		coverage_mask_view[idx] = data.get_pixel_coverage();
		depth_map_view[idx] = data.get_pixel_depth();
	});

	image_view.synchronize();
	coverage_mask_view.synchronize();
	depth_map_view.synchronize();
	

}