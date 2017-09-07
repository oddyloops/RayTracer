#include "rt_shader.h"
#include "math_util.h"
#include "vector_util.h"
#include "rt_wave_props.h"
using namespace rt;

rt_shader::rt_shader()
{

}

rt_shader::rt_shader(scene_database* db, vector<float> ambient_light, float ambient_intensity, vector<float> view_dir)
{
	m_ambient_light = ambient_light;
	m_ambient_intensity = ambient_intensity;
	m_db = db;
	m_view_dir = view_dir;
	m_visible = rt_visibility(m_db);
}

vector<float> rt_shader::compute_shade(intersection_record& rec, int generation)
{


	if (rec.get_geom_index() != -1)
	{
		rt_material mat = get_material_from_index(rec.get_material_index());
		vector <float> shade = { 0,0,0 };
		shade = compute_ambience(mat) + compute_diffuse(rec, mat, generation) + compute_specular(rec, mat, generation);
		return vector_util::clip_color(shade);
	}
	else {
		return pixel_data().get_default_color();
	}
}



vector<float> rt_shader::compute_ambience( rt_material& mat)
{
	return m_ambient_intensity * m_ambient_light * mat.get_ambient_color();
}

vector<float> rt_shader::compute_diffuse(intersection_record& rec, rt_material& mat, int generation)
{
	if (generation < 0)
	{
		return{ 0,0,0 };
	}
	vector<float> diffuse_color = { 0,0,0 };
	float nDotL;
	float percent_light=0;
	
	//directional lights
	for (int i = 0; i < m_db->get_num_directional_lights(); i++)
	{

		rt_directional_light& light = m_db->get_directional_light(i);
		nDotL =vector_util::dot(rec.get_normal_at_intersect(), vector_util::negate(light.get_direction()));
		if (rec.get_type() == rt_geometry_type::rectangle) {
			nDotL = math_util::abs(nDotL);
		}
		if (nDotL > 0) {
			percent_light = light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index());
			vector<float> d = (
				 percent_light *
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
			reflect_and_refract(rec, light.get_direction(), percent_light, mat, diffuse_color, generation);
		}
	}


	//point lights
	for (int i = 0; i < m_db->get_num_point_lights(); i++)
	{
		rt_point_light& light = m_db->get_point_light(i);
		nDotL =vector_util::dot(rec.get_normal_at_intersect(), vector_util::normalize(light.get_origin() - rec.get_intersection_position()));
		if (rec.get_type() == rt_geometry_type::rectangle) {
			nDotL = math_util::abs(nDotL);
		}
		if (nDotL > 0) {
			percent_light = light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index());
			diffuse_color = diffuse_color + (
				percent_light*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
			reflect_and_refract(rec, vector_util::normalize(rec.get_intersection_position() - light.get_origin()), percent_light, mat, diffuse_color, generation);
		}
	}

	//spot lights
	for (int i = 0; i < m_db->get_num_spot_lights(); i++)
	{
		rt_spot_light& light = m_db->get_spot_light(i);
		nDotL = vector_util::dot(rec.get_normal_at_intersect(), vector_util::normalize(light.get_origin() - rec.get_intersection_position()));
		if (rec.get_type() == rt_geometry_type::rectangle) {
			nDotL = math_util::abs(nDotL);
		}
		if (nDotL > 0) {
			percent_light = light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index());
			diffuse_color = diffuse_color + (
				percent_light*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
			reflect_and_refract(rec, vector_util::normalize( rec.get_intersection_position() - light.get_origin()), percent_light, mat, diffuse_color, generation);
		}
	}

	// area lights

	for (int i = 0; i < m_db->get_num_area_lights(); i++)
	{
		rt_area_light& light = m_db->get_area_light(i);
		nDotL = vector_util::dot(rec.get_normal_at_intersect(), vector_util::negate(light.get_direction()));
		if (rec.get_type() == rt_geometry_type::rectangle) {
			nDotL = math_util::abs(nDotL);
		}
		if (nDotL > 0) {
			percent_light = light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index());
			diffuse_color = diffuse_color + (
				percent_light*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
			reflect_and_refract(rec, light.get_direction(), percent_light, mat, diffuse_color, generation);
		}
		
	}

	return vector_util::clip_color(diffuse_color);
}

vector<float> rt_shader::compute_specular(intersection_record& rec, rt_material& mat, int generation)
{
	if (generation < 0)
	{
		return{ 0,0,0 };
	}
	if (mat.get_is_specular())
	{
		vector<float> specular_color = { 0,0,0 };
		float percent_light = 0;
		vector<float> reflected;
		float vDotR;
		for (int i = 0; i < m_db->get_num_directional_lights(); i++)
		{
			rt_directional_light& light = m_db->get_directional_light(i);
			reflected = rt_wave_props::reflect(rec.get_normal_at_intersect(), light.get_direction());
			vDotR = vector_util::dot(vector_util::negate(m_view_dir), reflected);
			if (rec.get_type() == rt_geometry_type::rectangle) {
				vDotR = math_util::abs(vDotR);
			}
			if (vDotR > 0) {
				percent_light = light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index());
				specular_color = specular_color + (
					percent_light*
					powf(vDotR, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
				reflect_and_refract(rec, light.get_direction(), percent_light, mat, specular_color, generation);
			}
		}


		//point lights
		for (int i = 0; i < m_db->get_num_point_lights(); i++)
		{
			rt_point_light& light = m_db->get_point_light(i);
			reflected = rt_wave_props::reflect(rec.get_normal_at_intersect(), vector_util::normalize(rec.get_intersection_position() - light.get_origin()));
			vDotR = vector_util::dot(vector_util::negate(m_view_dir), reflected); 
			if (rec.get_type() == rt_geometry_type::rectangle) {
				vDotR = math_util::abs(vDotR);
			}
			if (vDotR > 0) {
				percent_light = light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index());
				specular_color = specular_color + (
					percent_light *
					powf(vDotR, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
				reflect_and_refract(rec, vector_util::normalize(rec.get_intersection_position() - light.get_origin()), percent_light, mat, specular_color, generation);
			}
		}
		

		//spot lights
		for (int i = 0; i < m_db->get_num_spot_lights(); i++)
		{
			rt_spot_light& light = m_db->get_spot_light(i);
			reflected = rt_wave_props::reflect(rec.get_normal_at_intersect(), vector_util::normalize(rec.get_intersection_position() - light.get_origin()));
			vDotR = vector_util::dot(vector_util::negate(m_view_dir), reflected);
			if (rec.get_type() == rt_geometry_type::rectangle) {
				vDotR = math_util::abs(vDotR);
			}
			if (vDotR > 0) {
				percent_light = light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index());

				specular_color = specular_color + (
					percent_light*
					powf(vDotR, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
				reflect_and_refract(rec,vector_util::normalize(rec.get_intersection_position() - light.get_origin()), percent_light, mat, specular_color, generation);
			}
		}
		
		// area lights

		for (int i = 0; i < m_db->get_num_area_lights(); i++)
		{
			rt_area_light& light = m_db->get_area_light(i);
			reflected = rt_wave_props::reflect(rec.get_normal_at_intersect(), light.get_direction());
			vDotR = vector_util::dot(vector_util::negate(m_view_dir), reflected);
			if (rec.get_type() == rt_geometry_type::rectangle) {
				vDotR = math_util::abs(vDotR);
			}
			if (vDotR > 0) {
				percent_light = light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index());
				specular_color = specular_color + (
					percent_light*
					powf(vDotR, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
				reflect_and_refract(rec, light.get_direction(), percent_light, mat, specular_color, generation);
			}
		}

		return vector_util::clip_color(specular_color);
	}
	return{ 0,0,0 }; //do not compute specularity if not enabled
}

rt_material rt_shader::get_material_from_index(int mat_res_index)
{
	rt_material mat;
	for (int i = 0; i < m_db->get_num_materials(); i++)
	{
		if (m_db->get_material(i).get_resource_index() == mat_res_index)
		{
			mat = m_db->get_material(i);
			break;
		}
	}
	return mat;
}

vector<float> rt_shader::color_bounce_lights(vector<float>& light_dir,intersection_record& prev_rec, int generation)
{
	ray r = ray::create_ray_from_pt_dir(prev_rec.get_intersection_position(), light_dir);
	intersection_record rec;
	m_visible.compute_visibility(r, prev_rec.get_geom_index(), rec);
	vector<float> color = { 0,0,0 };
	if (rec.get_geom_index() != -1)
	{
		rt_material mat = get_material_from_index(rec.get_material_index());
		color = compute_diffuse(rec, mat, generation - 1) + compute_specular(rec, mat, generation - 1);
	}
	return color;
}

void rt_shader::reflect_and_refract(intersection_record current_rec, vector<float> light_dir, float percent_light, rt_material& mat, vector<float>& result_color
	, int generation)
{
	if (mat.get_is_reflective_refractive() && percent_light > FLT_EPSILON)
	{


		vector<float> reflected = rt_wave_props::reflect(current_rec.get_normal_at_intersect(), light_dir);
		vector<float> refracted = rt_wave_props::refract(current_rec.get_normal_at_intersect(), light_dir, 1, mat.get_refractive_index());

		vector<float> reflected_color = color_bounce_lights(reflected, current_rec, generation);
		vector<float> refracted_color = color_bounce_lights(refracted, current_rec, generation);
		vector<float> d = result_color * (1 - mat.get_transparency() - mat.get_reflectivity()) + reflected_color * mat.get_reflectivity() + refracted_color * mat.get_refractive_index();

		result_color = result_color + d;
	}
}