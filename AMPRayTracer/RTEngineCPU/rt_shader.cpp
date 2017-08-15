#include "rt_shader.h"
#include "vector_util.h"
using namespace rt;

rt_shader::rt_shader()
{

}

rt_shader::rt_shader(scene_database* db, vector<float> ambient_light, float ambient_intensity)
{
	m_ambient_light = ambient_light;
	m_ambient_intensity = ambient_intensity;
	m_db = db;
}

vector<float> rt_shader::compute_shade(intersection_record& rec, int generation)
{

	if (rec.get_geom_index() != -1)
	{
		float c = rec.get_geom_index() / 10.0;
		return{ c, c, c };
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
	//directional lights
	for (int i = 0; i < m_db->get_num_directional_lights(); i++)
	{
		rt_directional_light& light = m_db->get_directional_light(i);
		diffuse_color = diffuse_color + (
			light.percent_light(m_db->get_all_rectangles(),m_db->get_all_spheres(),rec.get_intersection_position(),rec.get_geom_index()) *
			vector_util::dot(rec.get_normal_at_intersect(),vector_util::negate(light.get_direction())) *
			mat.get_diffuse_color() * light.get_color()
			);
	}


	//point lights
	for (int i = 0; i < m_db->get_num_point_lights(); i++)
	{
		rt_point_light& light = m_db->get_point_light(i);
		diffuse_color = diffuse_color + (
			light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
			vector_util::dot(rec.get_normal_at_intersect(), light.get_origin() - rec.get_intersection_position()) *
			mat.get_diffuse_color() * light.get_color()
			);
	}

	//spot lights
	for (int i = 0; i < m_db->get_num_spot_lights(); i++)
	{
		rt_spot_light& light = m_db->get_spot_light(i);
		diffuse_color = diffuse_color + (
			light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
			vector_util::dot(rec.get_normal_at_intersect(), light.get_origin() - rec.get_intersection_position()) *
			mat.get_diffuse_color() * light.get_color()
			);
	}

	// area lights

	for (int i = 0; i < m_db->get_num_area_lights(); i++)
	{
		rt_area_light& light = m_db->get_area_light(i);
		diffuse_color = diffuse_color + (
			light.percent_light(m_db->get_all_rectangles(), m_db->get_all_spheres(), rec.get_intersection_position(), rec.get_geom_index())*
			vector_util::dot(rec.get_normal_at_intersect(), light.get_origin() - rec.get_intersection_position()) *
			mat.get_diffuse_color() * light.get_color()
			);
	}
}

vector<float> rt_shader::compute_specular(intersection_record& rec, rt_material& mat);