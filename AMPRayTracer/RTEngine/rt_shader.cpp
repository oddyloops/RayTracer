#include <amp_math.h>

#include "rt_shader.h"
#include "rt_wave_props.h"
#include "vector_amp.h"
#include "math_util.h"

using namespace concurrency::fast_math;
using namespace rt;

rt_shader::rt_shader() restrict(amp, cpu) {}

rt_shader::rt_shader(float_3 ambient_light, float ambient_intensity, float_3 view_dir) restrict(amp, cpu)
{
	m_ambient_light = ambient_light;
	m_ambient_intensity = ambient_intensity;
	m_view_dir = view_dir;
}




float_3 rt_shader::compute_shade(intersection_record& rec, int generation, array_view<rt_directional_light, 1>* m_dir_lights, array_view<rt_point_light, 1>* m_point_lights, array_view<rt_area_light, 1>* m_area_lights,
	array_view<rt_spot_light, 1>* m_spot_lights, array_view<rt_material, 1>* m_materials, array_view<rt_rectangle, 1>* m_rectangles, array_view<rt_sphere, 1>* m_spheres) restrict(amp)
{


	if (rec.get_geom_index() != -1)
	{
		float_3 shade = float_3(0,0,0);

		for (int i = 0; i < m_materials->extent.size(); i++)
		{

			int idx = (*m_materials)(index<1>(i)).get_resource_index();
			if (idx == rec.get_material_index())
			{
				rt_material& mat = (*m_materials)(index<1>(i));
				shade = compute_ambience(mat) + compute_diffuse(rec, mat,m_dir_lights,m_point_lights,m_area_lights,m_spot_lights,m_materials,m_rectangles,m_spheres) + compute_specular(rec, mat,
					m_dir_lights, m_point_lights, m_area_lights, m_spot_lights, m_materials, m_rectangles, m_spheres);
				break;
			}

		}


		return vector_amp::clip_color(shade);
	}
	else {
		return pixel_data().get_default_color();
	}
}

float_3 rt_shader::compute_ambience(rt_material& mat) restrict(amp)
{
	return m_ambient_intensity * m_ambient_light * mat.get_ambient_color();
}

float_3 rt_shader::compute_diffuse(intersection_record& rec, rt_material& mat, array_view<rt_directional_light, 1>* m_dir_lights, array_view<rt_point_light, 1>* m_point_lights, array_view<rt_area_light, 1>* m_area_lights,
	array_view<rt_spot_light, 1>* m_spot_lights, array_view<rt_material, 1>* m_materials, array_view<rt_rectangle, 1>* m_rectangles, array_view<rt_sphere, 1>* m_spheres) restrict(amp)
{
	float_3 diffuse_color =float_3(0,0,0);
	float nDotL;
	//directional lights
	for (int i = 0; i < m_dir_lights->extent.size(); i++)
	{
		rt_directional_light& light = (*m_dir_lights)(index<1>(i));
		nDotL = vector_amp::dot(rec.get_normal_at_intersect(), -light.get_direction());
		if (rec.get_type() == rt_geometry_type::rectangle)
		{
			nDotL = math_util::abs(nDotL);
		}
		if (nDotL > 0) {
			diffuse_color = diffuse_color + (
				light.percent_light(*m_rectangles, *m_spheres, rec.get_intersection_position(), rec.get_geom_index())*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
		}
	}

	
	//point lights
	for (int i = 0; i < m_point_lights->extent.size(); i++)
	{
		rt_point_light& light = (*m_point_lights)(index<1>(i));
		nDotL = vector_amp::dot(rec.get_normal_at_intersect(), vector_amp::normalize(light.get_origin() - rec.get_intersection_position()));
		if (rec.get_type() == rt_geometry_type::rectangle)
		{
			nDotL = math_util::abs(nDotL);
		}
		if (nDotL > 0) {
			diffuse_color = diffuse_color + (
				light.percent_light(*m_rectangles, *m_spheres, rec.get_intersection_position(), rec.get_geom_index())*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
		}
	}
	
	//spot lights
	for (int i = 0; i <  m_spot_lights->extent.size(); i++)
	{
		rt_spot_light& light = (*m_spot_lights)(index<1>(i));
		nDotL = vector_amp::dot(rec.get_normal_at_intersect(), vector_amp::normalize(light.get_origin() - rec.get_intersection_position()));
		if (rec.get_type() == rt_geometry_type::rectangle)
		{
			nDotL = math_util::abs(nDotL);
		}
		if (nDotL > 0) {
			diffuse_color = diffuse_color + (
				light.percent_light(*m_rectangles, *m_spheres, rec.get_intersection_position(), rec.get_geom_index())*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
		}
	}

	// area lights

	/*for (int i = 0; i < m_area_lights->extent.size(); i++)
	{
		rt_area_light& light = (*m_area_lights)(index<1>(i));
		nDotL = vector_amp::dot(rec.get_normal_at_intersect(), -light.get_direction());
		if (rec.get_type() == rt_geometry_type::rectangle)
		{
			nDotL = math_util::abs(nDotL);
		}
		if (nDotL > 0) {
			diffuse_color = diffuse_color + (
				light.percent_light(*m_rectangles, *m_spheres, rec.get_intersection_position(), rec.get_geom_index())*
				nDotL *
				mat.get_diffuse_color() *
				light.get_color()
				);
		}
	}*/
	
	return vector_amp::clip_color(diffuse_color);
}

float_3 rt_shader::compute_specular(intersection_record& rec, rt_material& mat, array_view<rt_directional_light, 1>* m_dir_lights, array_view<rt_point_light, 1>* m_point_lights, array_view<rt_area_light, 1>* m_area_lights,
	array_view<rt_spot_light, 1>* m_spot_lights, array_view<rt_material, 1>* m_materials, array_view<rt_rectangle, 1>* m_rectangles, array_view<rt_sphere, 1>* m_spheres) restrict(amp)
{
	if (mat.get_specular_color().r > 0)
	{ //flag for checking if specular color was set
		float_3 specular_color = float_3(0, 0, 0);
		float_3 reflected;
		float vDotR;
		for (int i = 0; i < m_dir_lights->extent.size(); i++)
		{
			rt_directional_light& light = (*m_dir_lights)(index<1>(i));
			reflected = rt_wave_props::reflect(rec.get_normal_at_intersect(), light.get_direction());
				
			vDotR = vector_amp::dot(-m_view_dir, reflected);
			if (rec.get_type() == rt_geometry_type::rectangle)
			{
				vDotR = math_util::abs(vDotR);
			}
			if (vDotR > 0) {
				specular_color = specular_color + (
					light.percent_light(*m_rectangles, *m_spheres, rec.get_intersection_position(), rec.get_geom_index())*
					powf(vDotR, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
			}
		}

		
		//point lights
		for (int i = 0; i < m_point_lights->extent.size(); i++)
		{
			rt_point_light& light = (*m_point_lights)(index<1>(i));
			reflected = rt_wave_props::reflect(rec.get_normal_at_intersect(), vector_amp::normalize(rec.get_intersection_position() - light.get_origin()));
			
			vDotR = vector_amp::dot(-m_view_dir, reflected);
			if (rec.get_type() == rt_geometry_type::rectangle)
			{
				vDotR = math_util::abs(vDotR);
			}
			if (vDotR > 0) {
				specular_color = specular_color + (
					light.percent_light(*m_rectangles, *m_spheres, rec.get_intersection_position(), rec.get_geom_index())*
					powf(vDotR, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
			}
		}
		

		//spot lights
		for (int i = 0; i <m_spot_lights->extent.size(); i++)
		{
			rt_spot_light& light = (*m_spot_lights)(index<1>(i));
			reflected = rt_wave_props::reflect(rec.get_normal_at_intersect(), vector_amp::normalize(rec.get_intersection_position() - light.get_origin()));
			
			vDotR = vector_amp::dot(-m_view_dir, reflected);
			if (rec.get_type() == rt_geometry_type::rectangle)
			{
				vDotR = math_util::abs(vDotR);
			}
			if (vDotR > 0) {
				specular_color = specular_color + (
					light.percent_light(*m_rectangles, *m_spheres, rec.get_intersection_position(), rec.get_geom_index())*
					powf(vDotR, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
			}
		}
		
		// area lights

		/*for (int i = 0; i < m_area_lights->extent.size(); i++)
		{
			rt_area_light& light = (*m_area_lights)(index<1>(i));
			halfway_vec = -(0.5f * (-(m_view_dir) + (-(light.get_direction()))));
			nDotH = vector_amp::dot(rec.get_normal_at_intersect(), halfway_vec);
			if (nDotH > 0) {
				specular_color = specular_color + (
					light.percent_light(*m_rectangles, *m_spheres, rec.get_intersection_position(), rec.get_geom_index())*
					powf(nDotH, mat.get_specularity()) *
					mat.get_specular_color() * light.get_color()
					);
			}
		}*/

		return vector_amp::clip_color(specular_color);
	}
	return float_3(0,0,0); //do not compute specularity if not enabled
}