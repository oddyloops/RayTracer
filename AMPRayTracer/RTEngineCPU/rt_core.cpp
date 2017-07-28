#include "rt_core.h"
#include "rt_geometry.h"
#include "vector_util.h"
#include <algorithm>


using namespace rt;


rt_core::rt_core(rt_camera camera,image_spec spec, int seed, int num_of_samples,scene_database db)
{
	m_camera = camera;
	m_camera.initialize_image(spec);
	srand(seed);
	m_num_of_samples = num_of_samples;
	m_db = db;
	m_visibility = rt_visibility(&m_db);
	m_shader = rt_shader(&m_db);
	
}



pixel_data rt_core::compute_pixel_data(int current_x, int current_y)
{
	ray r;
	vector<float> sample_position = m_camera.get_pixel_position(current_x,current_y);
	if (m_camera.get_ortho_mode_on()) {
		//orthographic projection
		r = ray::create_ray_from_pt_dir(sample_position, m_camera.get_view_dir());
	}
	else
	{
		//perspective projection
		r = ray(m_camera.get_eye(), sample_position);
	}
	
	intersection_record rec;
	//compute visibility
	m_visibility.compute_visibility(r, INVALID_INDEX, rec);
	
	//compute color
	vector<float> color = m_shader.compute_shade(rec, m_camera.get_generation());
	//compute mask
	float coverage_mask = (rec.get_geom_index() == INVALID_INDEX) ? 0 : 1;
	//compute depth_map
	float depth_map = 1.0f - min(1.0f,rec.get_hit_distance() / FAR_PLANE_DIST);
	return pixel_data( color, coverage_mask, depth_map);

}





