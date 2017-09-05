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

	m_normal = vector_amp::cross(v1, v2);
	m_normal = vector_amp::normalize(m_normal);
	ma = m_normal.x;
	mb = m_normal.y;
	mc = m_normal.z;

	md = -vector_amp::dot(m_normal, m_vertices[0]);

	if (math_util::abs(m_normal.x) > math_util::abs(m_normal.y))
	{
		/*normal x > normal y*/
		if (math_util::abs(m_normal.x) > math_util::abs(m_normal.z))
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
		if (math_util::abs(m_normal.y) > math_util::abs(m_normal.z))
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

	float v1v4 = math_util::clock_wise_angle(v1, v4, m_normal); 
	float v3v5 = math_util::clock_wise_angle(v3, v5, m_normal);
	float angle90 = 0.5f * PI;
	return (v1v4 < angle90 && v1v4 > 0) && (v3v5 < angle90 && v3v5 > 0); //clockwise angle between v1 and v4, AND between v3 and v5 must be within 0-90deg

}

int rt_rectangle::intersect(ray& ray, intersection_record& record) restrict(amp)
{
	float dist = 0;
	float_3 hitPt, n;

	n = m_normal;    // because ray/plane intersection may flip the normal!
	if (!ray_plane_intersection(ray, n, md, dist,m_vertices[0]))
		return false;

	/*
	* rectangle behind the ray or there are other closer intersections
	*/
	if ((dist < 0) || (dist > record.get_hit_distance()))
		return false;

	hitPt = ray.get_origin() + (ray.get_direction() * dist);

	/*
	* Now need to decide inside or outside
	*/
	if (!inside_polygon(hitPt))
		return false;

	record.update_record(dist, hitPt, n, ray, m_material_index, get_resource_index(),m_type);
	return true;

}

void rt_rectangle::get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp)
{
	//stubbed
	u = v = 0;
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

float_3 rt_rectangle::get_normal() restrict(amp)
{
	return m_normal;
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