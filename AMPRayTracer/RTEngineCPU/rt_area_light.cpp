#include "rt_area_light.h"
#include "math_util.h"
#include "vector_util.h"



using namespace rt_support::lights;

rt_area_light::rt_area_light( vector<float> direction, rt_rectangle lit_area, float padding,float drop_constant) : rt_light(light_type::AREA)
{

	m_direction = vector_util::normalize(direction);
	m_drop_constant = drop_constant;
	m_lit_area = lit_area;
	m_padding = padding;

	initialize_penumbra();
	
	
}

rt_area_light::rt_area_light(vector<float> direction, rt_rectangle lit_area, float padding, float drop_constant, vector<float> color) : rt_light(light_type::AREA)
{

	m_direction = vector_util::normalize(direction);
	m_drop_constant = drop_constant;
	m_lit_area = lit_area;
	m_color = color;
	m_padding = padding;
	initialize_penumbra();
}

rt_area_light::rt_area_light( vector<float> direction, rt_rectangle lit_area, float padding, float drop_constant, vector<float> color, float range, float att_frac, bool is_real_att) : rt_light(light_type::AREA)
{

	m_direction = vector_util::normalize(direction);
	m_drop_constant = drop_constant;
	m_lit_area = lit_area;
	m_color = color;
	m_padding = padding;
	m_range = range;
	m_att_frac = att_frac;
	m_realistic_att = is_real_att;
	initialize_penumbra();
}


void rt_area_light::initialize_penumbra()
{
	vector<float> pen_vertices[4];
	
	//hor_direction
	m_hor_direction = vector_util::normalize(m_lit_area.get_vertex(1) - m_lit_area.get_vertex(0));
	m_ver_direction = vector_util::normalize(m_lit_area.get_vertex(3) - m_lit_area.get_vertex(0));
	
	
	pen_vertices[0] = m_lit_area.get_vertex(0) - (m_padding * m_hor_direction) - (m_padding * m_ver_direction);
	pen_vertices[1] = m_lit_area.get_vertex(1) + (m_padding * m_hor_direction) - (m_padding * m_ver_direction);
	pen_vertices[2] = m_lit_area.get_vertex(2) + (m_padding * m_hor_direction) + (m_padding * m_ver_direction);
	pen_vertices[3] = m_lit_area.get_vertex(3) - (m_padding * m_hor_direction) + (m_padding * m_ver_direction);

	m_penumbra = rt_rectangle(pen_vertices);


}

///<summary>
///Determines the percentage of light a pixel gets based on intersection with the light ray
///</summary>
float rt_area_light::percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<float> geomPoint, int exceptGeomIndex)
{
	float percent = 1.0f;

	//determine if it is within the total fov
	ray r=ray::create_ray_from_pt_dir(geomPoint,vector_util::negate(m_direction));
	intersection_record rec;
	

	float dist = vector_util::dot(m_lit_area.get_vertex(0)- r.get_origin(),r.get_direction()); //distance between spot light and geometry
	intersection_record light_rec;
	
	if (m_penumbra.intersect(r,light_rec))
	{
		//within the coverage of the area light

		for (rt_rectangle& rect : rects)
		{
			if (rect.get_resource_index() != exceptGeomIndex)
			{
				if (rect.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
				{
					return 0.0f;
				}
			}
		}

		for (rt_sphere& sph : spheres)
		{
			if (sph.get_resource_index() != exceptGeomIndex)
			{
				if (sph.intersect(r, rec) && rec.get_hit_distance() > 0 && rec.get_hit_distance() < dist)
				{
					return 0.0f;
				}
			}
		}

		//no blockage
		if (m_range > 0)
		{
			percent = compute_attenuation(dist);
		}
		//is it outside the fully lit area
		if (!m_lit_area.intersect(r,light_rec))
		{
			//compute area light penumbra attenuation with cosine interpolation
			//determine if intersection happens within vertical or horizontal penumbra
			vector<float> hitPt = light_rec.get_intersection_position();
			float verDist = vector_util::point_line_distance(hitPt, m_lit_area.get_vertex(0), m_lit_area.get_vertex(3));
			float horDist = 0.0f;
			if (verDist <= m_padding)
			{
				//compute using vertical padding
				percent *= powf(cosf((verDist / m_padding) * 0.5f * PI), m_drop_constant);
			}
			else
			{
				horDist = vector_util::point_line_distance(hitPt, m_lit_area.get_vertex(0), m_lit_area.get_vertex(1));
				//compute using vertical padding
				percent *= powf(cosf((horDist / m_padding) * 0.5f * PI), m_drop_constant);
			}
		}

		return percent;
	}
	return 0.0f;
}