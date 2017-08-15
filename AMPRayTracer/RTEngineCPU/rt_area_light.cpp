#include "rt_area_light.h"
#include "math_util.h"
#include "vector_util.h"



using namespace rt_support::lights;

rt_area_light::rt_area_light(vector<float> origin, vector<float> direction, float vertical_tilt, float focus, float lit_width, float lit_height, float padding,float drop_constant) : rt_light(light_type::AREA)
{
	m_origin = origin;
	m_direction = direction;
	m_drop_constant = drop_constant;
	m_focus = focus;
	m_padding = padding;

	intiliaze_rectangles(vertical_tilt, focus, lit_width, lit_height);
	
	
}

rt_area_light::rt_area_light(vector<float> origin, vector<float> direction, float vertical_tilt, float focus, float lit_width, float lit_height, float padding, float drop_constant, vector<float> color) : rt_light(light_type::AREA)
{
	m_origin = origin;
	m_direction = direction;
	m_drop_constant = drop_constant;
	m_focus = focus;
	m_color = color;
	m_padding = padding;
	intiliaze_rectangles(vertical_tilt, focus, lit_width, lit_height);
}

rt_area_light::rt_area_light(vector<float> origin, vector<float> direction, float vertical_tilt, float focus, float lit_width, float lit_height, float padding, float drop_constant, vector<float> color, float range, float att_frac, bool is_real_att) : rt_light(light_type::AREA)
{
	m_origin = origin;
	m_direction = direction;
	m_drop_constant = drop_constant;
	m_focus = focus;
	m_color = color;
	m_padding = padding;
	m_range = range;
	m_att_frac = att_frac;
	m_realistic_att = is_real_att;
	intiliaze_rectangles(vertical_tilt, focus, lit_width, lit_height);
}


void rt_area_light::intiliaze_rectangles(float vertical_tilt, float focus, float lit_width, float lit_height)
{
	//compute horizontal and vertical directions for the rectangles
	//rotate y axis by the z-axis direction by the vertical tilt
	vector<float> up_direction = vector_util::normalize(matrix::transform({ 0,1,0 }, matrix::rotate_z(math_util::deg_to_rad(vertical_tilt))));
	m_ver_direction = up_direction;

	vector<float> side_direction = vector_util::normalize(vector_util::cross(m_direction, up_direction));
	m_hor_direction = side_direction;

	vector<float> rect_center = m_origin + focus * m_direction;

	//compute vertices for each rectangle
	vector<float> vertices[4];
	vertices[0] = { rect_center - 0.5f * lit_width * side_direction - 0.5f* lit_height * up_direction};
	vertices[1] = { rect_center - 0.5f * lit_width * side_direction + 0.5f* lit_height * up_direction };
	vertices[2] = { rect_center + 0.5f * lit_width * side_direction + 0.5f* lit_height * up_direction };
	vertices[3] = { rect_center + 0.5f * lit_width * side_direction - 0.5f* lit_height * up_direction };

	float penumbra_width = lit_width + m_padding;
	float penumbra_height = lit_width + m_padding;
	vector<float> outer_vertices[4];
	outer_vertices[0] = { rect_center - 0.5f * penumbra_width * side_direction - 0.5f* penumbra_height * up_direction };
	outer_vertices[1] = { rect_center - 0.5f * penumbra_width * side_direction + 0.5f* penumbra_height * up_direction };
	outer_vertices[2] = { rect_center + 0.5f * penumbra_width * side_direction + 0.5f* penumbra_height * up_direction };
	outer_vertices[3] = { rect_center + 0.5f * penumbra_width * side_direction - 0.5f* penumbra_height * up_direction };

	m_lit_area = rt_rectangle(vertices);
	m_penumbra = rt_rectangle(outer_vertices);


}

///<summary>
///Determines the percentage of light a pixel gets based on intersection with the light ray
///</summary>
float rt_area_light::percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<float> geomPoint, int exceptGeomIndex)
{
	float percent = 1.0f;

	//determine if it is within the total fov
	ray r(geomPoint, m_origin);
	intersection_record rec;
	

	float dist = vector_util::magnitude(m_origin - r.get_origin()); //distance between spot light and geometry
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