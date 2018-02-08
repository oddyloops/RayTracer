#include "rt_core.h"
#include "rt_geometry.h"




using namespace rt;


rt_core::rt_core(rt_camera camera, image_spec spec, int seed, int no_of_samples,float_3 ambient_light, float ambient_intensity) restrict(amp, cpu)
{
	m_camera = camera;
	m_rand = amp_lcg_rand(seed);
	m_num_of_samples = no_of_samples;
	m_shader = rt_shader(ambient_light, ambient_intensity, m_camera.get_view_dir());

}

pixel_data rt_core::compute_pixel_data(int current_x, int current_y, array_view<rt_geometry, 1> geom,
	array_view<rt_light, 1> lights, array_view<rt_material, 1> materials, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
	, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp)
{
	ray r;
	float_3 sample_position = m_camera.get_pixel_position(current_x, current_y);



	//compute color
	float_3 color = float_3(0, 0, 0);

	//compute mask
	float coverage_mask = 0;


	//compute depth_map
	float depth_map = 0;


	for (int i = 0; i < m_num_of_samples; i++)
	{

		float rndX = m_rand.rand_f();
		float rndY = m_rand.rand_f();

		float_3 real_sample_pos = sample_position + rndX * m_camera.get_pixel_dx() + rndY * m_camera.get_pixel_dy();

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
		m_visibility.compute_visibility(r, INVALID_INDEX, rec,&geom, bitmaps, scalars, f_bitmaps, f_scalars);
		color = color + m_shader.compute_shade(rec, m_camera.get_generation(),&lights,&materials,&geom,
			bitmaps,scalars,f_bitmaps,f_scalars);
		coverage_mask = coverage_mask + ((rec.get_geom_index() == INVALID_INDEX) ? 0 : 1);
		depth_map = depth_map + (1.0f - fminf(1.0f, rec.get_hit_distance() / FAR_PLANE_DIST));
	}

	color = color / m_num_of_samples;
	coverage_mask = coverage_mask / m_num_of_samples;
	depth_map = depth_map / m_num_of_samples;

	return pixel_data(color, coverage_mask, depth_map);
}


