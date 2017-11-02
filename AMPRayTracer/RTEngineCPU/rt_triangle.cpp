#include "rt_triangle.h"
#include "vector_util.h"

using namespace rt_support::geometries;

rt_triangle::rt_triangle() {}

rt_triangle::rt_triangle(vector<float> vertices[], matrix<float> xform)
{
	for (vector<float>* v = vertices; v!= nullptr; v++)
	{
		*v = matrix<float>::transform(*v, xform);
	}
	if (vertices + 2 == nullptr)
	{
		//not up to three vertices?
		throw exception("Triangle must have three vertices");
	}
	m_type = rt_geometry_type::triangle;
	for (int i = 0; i < 3; i++)
	{
		m_vertices[i] = vertices[i];
	}
	initialize_triangle();
}

rt_triangle::rt_triangle(vector<float> vertices[])
{
	if (vertices + 2 == nullptr)
	{
		//not up to three vertices?
		throw exception("Triangle must have three vertices");
	}
	m_type = rt_geometry_type::triangle;
	for (int i = 0; i < 3; i++)
	{
		m_vertices[i] = vertices[i];
	}
	initialize_triangle();
}

void rt_triangle::initialize_triangle()
{
	m_u_vec = m_vertices[1] - m_vertices[0];
	m_v_vec = m_vertices[2] - m_vertices[0];
	m_true_normal = vector_util::normalize(vector_util::cross(m_u_vec,m_v_vec));
	md = -vector_util::dot(m_true_normal, m_vertices[0]);
}


bool rt_triangle::intersect(ray& ray, intersection_record& record)
{
	float dist = 0;
	vector<float> hitPt, n;

	n = m_true_normal;    // because ray/plane intersection may flip the normal!
	if (!ray_plane_intersection(ray, n, md, dist, m_vertices[0]))
		return false;



	hitPt = ray.get_origin() + (ray.get_direction() * dist);

	/*
	* Now need to decide inside or outside
	*/
	float u, v =-1;
	if (!inside_polygon(hitPt,u,v))
		return false;

	vector<float> normal = n;

	get_uv(hitPt, { 0 }, u, v);
	if (!m_normal_map.is_null())
	{

		normal = vector_util::normalize(get_normal(u, v));

		if (vector_util::is_mirror_of(n, m_true_normal))
		{
			normal = vector_util::negate(normal);
		}

	}

	if (!m_bump_map.is_null())
	{
		//bump the point by a height b along the normal
		float b = m_bump_map.get_value(u, v);
		vector<float> old_hit_point = hitPt;
		hitPt = old_hit_point + b * normal;
		float dist_diff = vector_util::magnitude(hitPt - old_hit_point);
		dist += dist_diff;
	}

	record.update_record(dist, hitPt, normal, ray, m_material_index, get_resource_index(), m_type, u, v);
	return true;
}

bool rt_triangle::inside_polygon(vector<float> pt,float& u, float& v)
{
	vector<float> w = pt - m_vertices[0];  // w = rU + tV where r + t <= 1 (barycentric coordinates)
	
	//after crossing both sides of the equation with v, we eliminate t and vice-versa for u

	//check if r is positive by using the dot product to see if the quotient vectors point in opposite directions
	vector<float> vw = vector_util::cross(m_v_vec, w);
	vector<float> vu = vector_util::cross(m_v_vec, m_u_vec);
	vector<float> uw = vector_util::cross(m_u_vec, w);
	vector<float> uv = vector_util::cross(m_u_vec, m_v_vec);
	bool positive_r = vector_util::dot(vw,vu) >= 0;
	bool positive_t = vector_util::dot(uw,uv) >= 0;
	
	if (!positive_r || !positive_t)
		return false;

	//check if r+t <= 1
	float uv_size = vector_util::magnitude(uv);
	float uw_size = vector_util::magnitude(uw);
	float vw_size = vector_util::magnitude(vw);

	float r = vw_size / uv_size;
	float t = uw_size / uv_size;
	vector<float> bc = { r, 1 - (r + t), t };
	bool inside = r + t <= 1;
	if (inside)
	{
		get_uv(pt, bc, u, v);
	}

	return inside;

}


void rt_triangle::get_uv(vector<float> pt, vector<float> bc, float& u, float& v)
{
	u = bc[1] * 1 + bc[2] * 0.5;
	v = bc[2] * 1;
}

vector<float> rt_triangle::get_position(float u, float v)
{
	return{ 0,0,0 };
}

vector<float> rt_triangle::get_center()
{
	return  (m_vertices[0] + m_vertices[1] + m_vertices[2]) / 3;
}



vector<float> rt_triangle::get_max()
{
	return { 0,0,0 };
}

vector<float> rt_triangle::get_min()
{
	return { 0,0,0 };
}

vector<float> rt_triangle::get_vertex(int i)
{
	return m_vertices[i];
}