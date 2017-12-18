#include <math.h>
#include "rt_rectangle.h"
#include "math_util.h"
#include "matrix_amp.h"
#include "vector_amp.h"


using namespace rt_support::geometries;

rt_rectangle::rt_rectangle()  restrict(amp, cpu) {}

rt_rectangle::rt_rectangle(float_3 vertices[], array<float,2> xform)
{
	m_type = rt_geometry_type::rectangle;
	if (vertices + 3 == nullptr) //not up to four vertices?
	{
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		m_vertices[i] = vertices[i];
	}
	
		for (int i = 0; i < 4; i++)
		{
			m_vertices[i] = matrix_amp::transform(m_vertices[i], xform);
		}
	
	initialize_rectangle();
}

rt_rectangle::rt_rectangle(float_3 vertices[])  restrict(amp, cpu)
{
	m_type = rt_geometry_type::rectangle;
	m_material_index = 0;

	for (int i = 0; i < 4; i++)
	{
		m_vertices[i] = vertices[i];
	}

	initialize_rectangle();
}

void rt_rectangle::initialize_rectangle() restrict(amp,cpu)
{
	float_3 v1, v2;
	m_u_vec = v1 = m_vertices[1] - m_vertices[0]; //horizontal lower corner - origin
	m_v_vec = v2 = m_vertices[3] - m_vertices[0];
	m_u_size = vector_amp::magnitude(m_u_vec);
	m_v_size = vector_amp::magnitude(m_v_vec);
	m_u_vec = vector_amp::normalize(m_u_vec);
	m_v_vec = vector_amp::normalize(m_v_vec);

	m_true_normal = vector_amp::cross(v1, v2);
	m_true_normal = vector_amp::normalize(m_true_normal);
	ma = m_true_normal.x;
	mb = m_true_normal.y;
	mc = m_true_normal.z;

	md = -vector_amp::dot(m_true_normal, m_vertices[0]);

	if (math_util::abs(m_true_normal.x) > math_util::abs(m_true_normal.y))
	{
		/*normal x > normal y*/
		if (math_util::abs(m_true_normal.x) > math_util::abs(m_true_normal.z))
		{
			/*normal x > both y and z*/
			m_u_axis_index = 1;
			m_v_axis_index = 2;
		}
		else {
			/* y < x < z */
			m_u_axis_index = 0;
			m_v_axis_index = 1;
		}
	}
	else
	{
		/* y > x*/
		if (math_util::abs(m_true_normal.y) > math_util::abs(m_true_normal.z))
		{
			/* y  > x and z*/
			m_u_axis_index = 0;
			m_v_axis_index = 2;
		}
		else {
			/* x < y <z*/
			m_u_axis_index = 0;
			m_v_axis_index = 1;
		}
	}
	
}

int rt_rectangle::inside_polygon(float_3 pt) restrict(amp)
{
	float_3 v3 = m_vertices[3] - m_vertices[2];
	float_3 v4 = pt - m_vertices[0];
	float_3 v5 = pt - m_vertices[2];
	float_3 v1 = m_u_vec;
	v3 = vector_amp::normalize(v3);
	v4 = vector_amp::normalize(v4);
	v5 = vector_amp::normalize(v5);

	float v1v4 = math_util::clock_wise_angle(v1, v4, m_true_normal); 
	float v3v5 = math_util::clock_wise_angle(v3, v5, m_true_normal);
	float angle90 = 0.5f * PI;
	return (v1v4 < angle90 && v1v4 > 0) && (v3v5 < angle90 && v3v5 > 0); //clockwise angle between v1 and v4, AND between v3 and v5 must be within 0-90deg

}

int rt_rectangle::intersect(ray& r, intersection_record& record) restrict(amp)
{
	float dist = 0;
	float_3 hitPt, n;

	n = m_true_normal;    // because ray/plane intersection may flip the normal!
	if (!ray_plane_intersection(r, n, md, dist, m_vertices[0]))
		return false;

	/*
	* rectangle behind the ray or there are other closer intersections
	*/
	if ((dist < 0) || (dist > record.get_hit_distance()))
		return false;

	hitPt = r.get_origin() + (r.get_direction() * dist);

	/*
	* Now need to decide inside or outside
	*/
	if (!inside_polygon(hitPt))
		return false;
	
	record.update_record(dist, hitPt, n, r, m_material_index, get_resource_index(), m_type);
	return true;

}

int rt_rectangle::intersect(ray& r, intersection_record& record, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
	, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp)
{
	if (intersect(r, record) == false)
	{
		return false;
	}

	float_3 n = record.get_normal_at_intersect();
	float_3 normal = n;
	float_3 hitPt = record.get_intersection_position();
	float dist = record.get_hit_distance();
	float u, v;
	get_uv(hitPt, { 0 }, u, v);



	if (!m_normal_map.is_null())
	{

		normal = vector_amp::normalize(get_normal(u, v,bitmaps,scalars));

		if (vector_amp::is_mirror_of(n, m_true_normal))
		{
			normal = -normal;
		}

	}

	if (!m_bump_map.is_null())
	{
		//bump the point by a height b along the normal
		float b = m_bump_map.get_value(u, v,f_bitmaps,f_scalars);
		float_3 old_hit_point = hitPt;
		hitPt = old_hit_point + b * normal;

		float dist_diff = vector_amp::magnitude(hitPt - old_hit_point);
		dist += dist_diff;
	}
	record.force_update_record(dist, hitPt, n, r, m_material_index, get_resource_index(),m_type);
	return true;

}

void rt_rectangle::get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp)
{
	float_3 pt_norm = pt - m_vertices[0];
	u = vector_amp::dot(pt_norm, m_u_vec) / m_u_size; //project vec on u dir
	v = vector_amp::dot(pt_norm, m_v_vec) / m_v_size; // project vec on v dir
}

float_3 rt_rectangle::get_position(float u, float v) restrict(amp)
{
	return float_3();
}

float_3 rt_rectangle::get_center() restrict(amp)
{
	//average of all vertices
	float_3 c = m_vertices[0];
	for (int i = 1; i < 4; i++)
	{
		c += m_vertices[i];
	}
	return c / 4;
}

float rt_rectangle::get_u_size() restrict(amp)
{
	return m_u_size;
}

float rt_rectangle::get_v_size() restrict(amp)
{
	return m_v_size;
}


float_3 rt_rectangle::get_max() restrict(amp)
{
	//stub
	return float_3();
}

float_3 rt_rectangle::get_min() restrict(amp)
{
	//stub
	return float_3();
}

float_3 rt_rectangle::get_vertex(int i) restrict(amp,cpu)
{
	return m_vertices[i];
}