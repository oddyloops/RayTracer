#include "rt_triangle.h"
#include "vector_amp.h"
#include "matrix_amp.h"

using namespace rt_support::geometries;

rt_triangle::rt_triangle() restrict(amp,cpu) {}

rt_triangle::rt_triangle(float_3 vertices[], concurrency::array<float,2> xform)
{
	for (float_3* v = vertices; v != nullptr; v++)
	{
		*v = matrix_amp::transform(*v, xform);
	}
	if (vertices + 2 == nullptr)
	{
		//not up to three vertices?
		throw std::exception("Triangle must have three vertices");
	}
	m_type = rt_geometry_type::triangle;
	for (int i = 0; i < 3; i++)
	{
		m_vertices[i] = vertices[i];
	}
	initialize_triangle();
}

rt_triangle::rt_triangle(float_3 vertices[]) restrict(amp,cpu)
{
	if (vertices + 2 == nullptr)
	{
		//not up to three vertices?
		return;
	}
	m_type = rt_geometry_type::triangle;
	for (int i = 0; i < 3; i++)
	{
		m_vertices[i] = vertices[i];
	}
	initialize_triangle();
}

void rt_triangle::initialize_triangle() restrict(amp,cpu)
{
	m_u_vec = m_vertices[1] - m_vertices[0];
	m_v_vec = m_vertices[2] - m_vertices[0];
	m_normal = vector_amp::normalize(vector_amp::cross(m_u_vec, m_v_vec));
	md = -vector_amp::dot(m_normal, m_vertices[0]);
}


int rt_triangle::intersect(ray& ray, intersection_record& record) restrict(amp)
{
	float dist = 0;
	float_3 hitPt, n;

	n = m_normal;    // because ray/plane intersection may flip the normal!
	if (!ray_plane_intersection(ray, n, md, dist, m_vertices[0]))
		return false;



	hitPt = ray.get_origin() + (ray.get_direction() * dist);

	/*
	* Now need to decide inside or outside
	*/
	if (!inside_polygon(hitPt))
		return false;

	record.update_record(dist, hitPt, n, ray, m_material_index, get_resource_index(), m_type);
	return true;
}

int rt_triangle::inside_polygon(float_3 pt) restrict(amp)
{
	float_3 w = pt - m_vertices[0];  // w = rU + tV where r + t <= 1 (barycentric coordinates)

										   //after crossing both sides of the equation with v, we eliminate t and vice-versa for u

										   //check if r is positive by using the dot product to see if the quotient vectors point in opposite directions
	float_3 vw = vector_amp::cross(m_v_vec, w);
	float_3 vu = vector_amp::cross(m_v_vec, m_u_vec);
	float_3 uw = vector_amp::cross(m_u_vec, w);
	float_3 uv = vector_amp::cross(m_u_vec, m_v_vec);
	bool positive_r = vector_amp::dot(vw, vu) >= 0;
	bool positive_t = vector_amp::dot(uw, uv) >= 0;

	if (!positive_r || !positive_t)
		return false;

	//check if r+t <= 1
	float uv_size = vector_amp::magnitude(uv);
	float uw_size = vector_amp::magnitude(uw);
	float vw_size = vector_amp::magnitude(vw);

	float r = vw_size / uv_size;
	float t = uw_size / uv_size;

	return r + t <= 1;

}


void rt_triangle::get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp)
{

}

float_3 rt_triangle::get_position(float u, float v) restrict(amp)
{
	return{ 0,0,0 };
}

float_3 rt_triangle::get_center() restrict(amp)
{
	return  (m_vertices[0] + m_vertices[1] + m_vertices[2]) / 3;
}

float_3 rt_triangle::get_normal() restrict(amp)
{
	return m_normal;
}

float_3 rt_triangle::get_max() restrict(amp)
{
	return { 0,0,0 };
}

float_3 rt_triangle::get_min() restrict(amp)
{
	return { 0,0,0 };
}

float_3 rt_triangle::get_vertex(int i) restrict(amp)
{
	return m_vertices[i];
}