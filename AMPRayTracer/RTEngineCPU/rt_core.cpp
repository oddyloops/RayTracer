#include "rt_core.h"
#include "rt_geometry.h"
#include <time.h>
#include "vector_util.h"



using namespace rt;


rt_core::rt_core(rt_camera camera, int seed, int num_of_samples)
{
	m_camera = rt_camera(camera.get_eye(),camera.get_at(),camera.get_up(),camera.get_fov(),camera.get_focus());
	srand(time(0));
	m_num_of_samples = num_of_samples;
	
}

pixel_data rt_core::compute_pixel_data(int current_x, int current_y, vector<rt_sphere> spheres, vector<rt_rectangle> rectangles)
{
	// calculate the current position of the pixel
	vector<float> pixel_3d_position = m_camera.get_rt_camera_near_plane().get_upper_left() + (current_x * m_camera.get_rt_camera_near_plane().get_pixel_width() * m_camera.get_orthonormal_basis().get_side_vector()) -
		(current_y * m_camera.get_rt_camera_near_plane().get_pixel_height() * m_camera.get_orthonormal_basis().get_up_vector());

	// compute pixel data
	return compute_pixel_data_with_samples(pixel_3d_position,spheres,rectangles);
}


pixel_data rt_core::compute_pixel_data_with_samples(vector<float> position,  vector<rt_sphere> spheres, vector<rt_rectangle> rectangles)
{
	//pixel upper left
	pixel_data initial_pixel = compute_sample_pixel_data(position,spheres,rectangles);
	vector<float> cumulative_color = { 0,0,0 }; //used to compute average color from super sampling
	float cumulative_coverage = 0; //used to compute average coverage mask
	float cumulative_depth = 0; //used to compute average depth map
	for (int i = 0; i < m_num_of_samples; i++)
	{
		vector<float> new_position = initial_pixel.get_pixel_position() + ((1.0f * rand()/RAND_MAX) * m_camera.get_rt_camera_near_plane().get_pixel_width() * m_camera.get_orthonormal_basis().get_side_vector())
			- ((1.0f * rand() / RAND_MAX) * m_camera.get_rt_camera_near_plane().get_height() * m_camera.get_orthonormal_basis().get_up_vector());
		
		pixel_data new_pixel = compute_sample_pixel_data(new_position,spheres,rectangles);
		
		cumulative_color = cumulative_color  + new_pixel.get_pixel_color();
		cumulative_coverage = cumulative_coverage + new_pixel.get_pixel_coverage();
		cumulative_depth = cumulative_depth + new_pixel.get_pixel_depth();
	}
	cumulative_color = cumulative_color/m_num_of_samples;
	cumulative_coverage = cumulative_coverage/m_num_of_samples;
	cumulative_depth = cumulative_depth/ m_num_of_samples;

	return pixel_data(initial_pixel.get_pixel_position(), cumulative_color, cumulative_coverage, cumulative_depth, initial_pixel.get_hit_distance());


}

pixel_data rt_core::compute_sample_pixel_data(vector<float> sample_position, vector<rt_sphere> spheres, vector<rt_rectangle> rectangles)
{
	ray r;
	if (m_camera.get_ortho_mode_on()) {
		//orthographic projection
		r = ray::create_ray_from_pt_dir(sample_position, m_camera.get_orthonormal_basis().get_view_direction());
	}
	else
	{
		//perspective projection
		r = ray(m_camera.get_eye(), sample_position);
	}
	
	intersection_record rec;
	//compute visibility
	m_visibility.compute_visibility(r, INVALID_INDEX, rec,spheres,rectangles);
	
	//compute color
	vector<float> color = m_shader.compute_shade(rec, m_camera.get_generation());
	//compute mask
	float coverage_mask = (rec.get_geom_index() == INVALID_INDEX) ? 0 : 1;
	//compute depth_map
	float depth_map = 1.0 - (rec.get_hit_distance() / FAR_PLANE_DIST);
	return pixel_data(sample_position, color, coverage_mask, depth_map, rec.get_hit_distance());

}





