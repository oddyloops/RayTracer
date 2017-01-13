#include "rt_core.h"
#include "rt_geometry.h"




using namespace rt;


rt_core::rt_core(rt_camera camera, int seed) restrict(amp,cpu)
{
	m_camera = rt_camera(camera.get_eye(),camera.get_at(),camera.get_up(),camera.get_fov(),camera.get_focus());
	m_rand = amp_lcg_rand(seed);
	
}

pixel_data rt_core::compute_pixel_data(int current_x, int current_y, int num_of_samples, array_view<rt_sphere, 1>& spheres, array_view<rt_rectangle, 1>& rectangles) restrict(amp)
{
	// calculate the current position of the pixel
	float_3 pixel_3d_position = m_camera.get_rt_camera_near_plane().get_upper_left() + (current_x * m_camera.get_rt_camera_near_plane().get_pixel_width() * m_camera.get_orthonormal_basis().get_side_vector()) -
		(current_y * m_camera.get_rt_camera_near_plane().get_pixel_height() * m_camera.get_orthonormal_basis().get_up_vector());

	// compute pixel data
	return compute_pixel_data_with_samples(pixel_3d_position, num_of_samples,spheres,rectangles);
}


pixel_data rt_core::compute_pixel_data_with_samples(float_3 position, int num_of_samples, array_view<rt_sphere, 1>& spheres, array_view<rt_rectangle, 1>& rectangles) restrict(amp)
{
	//pixel upper left
	pixel_data initial_pixel = compute_sample_pixel_data(position,spheres,rectangles);
	float_3 cumulative_color(0,0,0); //used to compute average color from super sampling
	float cumulative_coverage = 0; //used to compute average coverage mask
	float cumulative_depth = 0; //used to compute average depth map
	for (int i = 0; i < num_of_samples; i++)
	{
		float_3 new_position = initial_pixel.get_pixel_position() + (m_rand.rand_f() * m_camera.get_rt_camera_near_plane().get_pixel_width() * m_camera.get_orthonormal_basis().get_side_vector())
			- (m_rand.rand_f() * m_camera.get_rt_camera_near_plane().get_height() * m_camera.get_orthonormal_basis().get_up_vector());
		
		pixel_data new_pixel = compute_sample_pixel_data(new_position,spheres,rectangles);
		
		cumulative_color += new_pixel.get_pixel_color();
		cumulative_coverage += new_pixel.get_pixel_coverage();
		cumulative_depth += new_pixel.get_pixel_depth();
	}
	cumulative_color /= num_of_samples;
	cumulative_coverage /= num_of_samples;
	cumulative_depth /= num_of_samples;

	return pixel_data(initial_pixel.get_pixel_position(), cumulative_color, cumulative_coverage, cumulative_depth, initial_pixel.get_hit_distance());


}

pixel_data rt_core::compute_sample_pixel_data(float_3 sample_position, array_view<rt_sphere, 1>& spheres, array_view<rt_rectangle, 1>& rectangles) restrict(amp)
{
	ray r = ray(m_camera.get_eye(), sample_position);
	intersection_record rec;
	//compute visibility
	compute_visibility(r, INVALID_INDEX, rec,spheres,rectangles);
	
	//compute color
	float_3 color = compute_shade(rec, m_camera.get_generation());
	//compute mask
	float coverage_mask = (rec.get_geom_index() == INVALID_INDEX) ? 0 : 1;
	//compute depth_map
	float depth_map = 1.0 - (rec.get_hit_distance() / FAR_PLANE_DIST);
	return pixel_data(sample_position, color, coverage_mask, depth_map, rec.get_hit_distance());

}


void rt_core::compute_visibility(ray r, int except_geom_index, intersection_record& rec,array_view<rt_sphere, 1>& spheres, array_view<rt_rectangle, 1>& rectangles) restrict(amp)
{
	for (int i = 0; i < spheres.extent.size(); i++)
	{
		if (i != except_geom_index)
		{
			index<1> idx(i);
			spheres[idx].intersect(r, rec);
		}
	}

	for (int i = 0; i < rectangles.extent.size(); i++)
	{
		if (i != except_geom_index)
		{
			index<1> idx(i);
			rectangles[idx].intersect(r, rec);
		}
	}
	
}


float_3 rt_core::compute_shade(intersection_record rec, int generation) restrict(amp)
{
	if (rec.get_geom_index() != INVALID_INDEX)
	{
		float c = rec.get_geom_index() / 10.0;
		return float_3(c, c, c);
	}
	else {
		return pixel_data().get_default_color();
	}
}