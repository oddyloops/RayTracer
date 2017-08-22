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
}

vector<float> rt_shader::compute_shade(intersection_record& rec, int generation)
{


	if (rec.get_geom_index() != -1)
	{
		vector <float> shade = { 0,0,0 };
		
		for (int i = 0; i < m_db->get_num_materials(); i++)
		{
			
			int index = m_db->get_material(i).get_resource_index();
			if (index == rec.get_material_index())
			{
				rt_material& mat = m_db->get_material(i);
				shade = compute_ambience(mat)+ compute_diffuse(rec, mat) + compute_specular(rec,mat);
				break;
			}
			
		}

	
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

vector<float> rt_shader::compute_diffuse(intersection_record& rec, rt_material& mat)
{
	vector<float> diffuse_color = { 0,0,0 };
	float nDotL;
	//directional lights
	for (int i = 0; i < m_db->get_num_directional_lights(); i++)
	{
		rt_directional_light& light = m_db->get_directional_light(i);
		nDotL =vector_util::dot(rec.get_normal_at_intersect(), vector_util::negate(light.get_direction()));
		if (nDotL > 0) {
			diffuse_color = diffuse_color + (
				light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
		}
	}


	//point lights
	for (int i = 0; i < m_db->get_num_point_lights(); i++)
	{
		rt_point_light& light = m_db->get_point_light(i);
		nDotL = math_util::abs(vector_util::dot(rec.get_normal_at_intersect(), vector_util::normalize(light.get_origin() - rec.get_intersection_position())));
		if (nDotL > 0) {
			diffuse_color = diffuse_color + (
				light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
		}
	}

	//spot lights
	for (int i = 0; i < m_db->get_num_spot_lights(); i++)
	{
		rt_spot_light& light = m_db->get_spot_light(i);
		nDotL = vector_util::dot(rec.get_normal_at_intersect(), vector_util::normalize(light.get_origin() - rec.get_intersection_position()));

		if (nDotL > 0) {
			diffuse_color = diffuse_color + (
				light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
		}
	}

	// area lights

	for (int i = 0; i < m_db->get_num_area_lights(); i++)
	{
		rt_area_light& light = m_db->get_area_light(i);
		nDotL = vector_util::dot(rec.get_normal_at_intersect(), vector_util::negate(light.get_direction()));
		if (nDotL > 0) {
			diffuse_color = diffuse_color + (
				light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
		}
	}

	return vector_util::clip_color(diffuse_color);
}

vector<float> rt_shader::compute_specular(intersection_record& rec, rt_material& mat)
{
	if (mat.get_specular_color().size() > 0)
	{
		vector<float> specular_color = { 0,0,0 };
	
		
		vector<float> halfway_vec;
		float nDotH;
		for (int i = 0; i < m_db->get_num_directional_lights(); i++)
		{
			rt_directional_light& light = m_db->get_directional_light(i);
			halfway_vec = vector_util::negate(0.5f * (vector_util::negate(m_view_dir) + vector_util::negate(light.get_direction())));
			nDotH = vector_util::dot(rec.get_normal_at_intersect(), halfway_vec);
			if (nDotH > 0) {
				specular_color = specular_color + (
					light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
					powf(nDotH, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
			}
		}


		//point lights
		for (int i = 0; i < m_db->get_num_point_lights(); i++)
		{
			rt_point_light& light = m_db->get_point_light(i);
			halfway_vec = vector_util::negate(0.5f * (vector_util::negate(m_view_dir) + vector_util::normalize(light.get_origin() - rec.get_intersection_position())));
			nDotH = vector_util::dot(rec.get_normal_at_intersect(), halfway_vec);
			if (nDotH > 0) {
				specular_color = specular_color + (
					light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
					powf(nDotH, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
			}
		}
		

		//spot lights
		for (int i = 0; i < m_db->get_num_spot_lights(); i++)
		{
			rt_spot_light& light = m_db->get_spot_light(i);
			halfway_vec = vector_util::negate(0.5f * (vector_util::negate(m_view_dir) + vector_util::normalize(light.get_origin() - rec.get_intersection_position())));
			nDotH = vector_util::dot(rec.get_normal_at_intersect(), halfway_vec);
			if (nDotH > 0) {
				specular_color = specular_color + (
					light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
					powf(nDotH, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
			}
		}
		
		// area lights

		for (int i = 0; i < m_db->get_num_area_lights(); i++)
		{
			rt_area_light& light = m_db->get_area_light(i);
			halfway_vec = vector_util::negate(0.5f * (vector_util::negate(m_view_dir) + vector_util::negate(light.get_direction())));
			nDotH = vector_util::dot(rec.get_normal_at_intersect(), halfway_vec);
			if (nDotH > 0) {
				specular_color = specular_color + (
					light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
					powf(nDotH, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
			}
		}

		return vector_util::clip_color(specular_color);
	}
	return{ 0,0,0 }; //do not compute specularity if not enabled
}