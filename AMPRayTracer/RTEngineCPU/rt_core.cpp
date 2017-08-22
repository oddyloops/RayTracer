#include "rt_core.h"
#include "rt_geometry.h"
#include "vector_util.h"
#include "math_util.h"
#include <algorithm>
#include <time.h>


using namespace rt;


rt_core::rt_core(rt_camera camera, image_spec spec, int seed, int num_of_samples, scene_database db, vector<float> ambient_light, float ambient_intensity)
{

	m_camera = camera;
	srand(seed);
	m_num_of_samples = num_of_samples;
	m_db = db;
	m_visibility = rt_visibility(&m_db);
	m_shader = rt_shader(&m_db,ambient_light,ambient_intensity,camera.get_view_dir());

}



pixel_data rt_core::compute_pixel_data(int current_x, int current_y)
{
	ray r;
	vector<float> sample_position = m_camera.get_pixel_position(current_x, current_y);

	
	//compute color
	vector<float> color = { 0,0,0, };

	//compute mask
	float coverage_mask = 0;


	//compute depth_map
	float depth_map = 0;

	
	for (int i = 0; i < m_camera.get_image_spec().get_samples_per_pixel(); i++)
	{

		float rndX = math_util::rnd0to1();
		float rndY = math_util::rnd0to1();

		vector<float> real_sample_pos = sample_position + rndX * m_camera.get_pixel_dx() + rndY * m_camera.get_pixel_dy();

		if (m_camera.get_ortho_mode_on()) 
		{
			//orthographic projection
			r = ray::create_ray_from_pt_dir(real_sample_pos, m_camera.get_view_dir());
		}
		else
		{
			//perspective projection
			r = ray(m_camera.get_eye(), real_sample_pos);
		}

		intersection_record rec;
		//compute visibility
		m_visibility.compute_visibility(r, INVALID_INDEX, rec);
		color = color + m_shader.compute_shade(rec, m_camera.get_generation());
		coverage_mask = coverage_mask + ((rec.get_geom_index() == INVALID_INDEX) ? 0 : 1);
		depth_map = depth_map + (1.0f - min(1.0f, rec.get_hit_distance() / FAR_PLANE_DIST));
	}

	color = color / m_camera.get_image_spec().get_samples_per_pixel();
	coverage_mask = coverage_mask / m_camera.get_image_spec().get_samples_per_pixel();
	depth_map = depth_map / m_camera.get_image_spec().get_samples_per_pixel();

	return pixel_data(color, coverage_mask, depth_map);

}





