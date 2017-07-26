#include <math.h>
#include "rt_rectangle.h"
#include "math_util.h"
#include "matrix.h"
#include "vector_util.h"


using namespace rt_support::geometries;

rt_rectangle::rt_rectangle(vector<float> vertices[], int material_index, matrix xform, int has_transform)
{
	m_type = rt_geometry_type::rectangle;
	if (vertices + 3 == nullptr) //not up to four vertices?
	{
		return;
	}
	m_material_index = material_index;
	for (int i = 0; i < 4; i++)
	{
		m_vertices[i] = vertices[i];
	}
	if (has_transform)
	{
		for (int i = 0; i < 4; i++)
		{
			m_vertices[i] = matrix::transform(m_vertices[i], xform);
		}
	}
	initialize_rectangle();
}

rt_rectangle::rt_rectangle(vector<float> vertices[])
{
	m_type = rt_geometry_type::rectangle;
	m_material_index = 0;

	for (int i = 0; i < 4; i++)
	{
		m_vertices[i] = vertices[i];
	}

	initialize_rectangle();
}

void rt_rectangle::initialize_rectangle()
{
	
	m_u_vec = m_vertices[1] - m_vertices[0];
	m_v_vec = m_vertices[3] - m_vertices[0];
	m_u_size = vector_util::magnitude(m_u_vec);
	m_v_size = vector_util::magnitude(m_v_vec);
	m_normal = vector_util::normalize(vector_util::cross(m_u_vec, m_v_vec));

	//compute D for the plane equation using crammer's rule for the simultaneous equation
	ma = (m_vertices[0][1] * (m_vertices[1][2] - m_vertices[2][2]))
		+ (m_vertices[1][1] * (m_vertices[2][2] - m_vertices[0][2]))
		+ (m_vertices[2][1] * (m_vertices[0][2] - m_vertices[1][2]));
	mb = (m_vertices[0][2] * (m_vertices[1][0] - m_vertices[2][0]))
		+ (m_vertices[1][2] * (m_vertices[2][0] - m_vertices[0][0]))
		+ (m_vertices[2][2] * (m_vertices[0][0] - m_vertices[1][0]));
	mc = (m_vertices[0][0] * (m_vertices[1][1] - m_vertices[2][1]))
		+ (m_vertices[1][0] * (m_vertices[2][1] - m_vertices[0][1]))
		+ (m_vertices[2][0] * (m_vertices[0][1] - m_vertices[1][1]));

	md = (-m_vertices[0][0] * (m_vertices[1][1] * m_vertices[2][2] - m_vertices[2][1] * m_vertices[1][2]))
		- (m_vertices[1][0] * (m_vertices[2][1] * m_vertices[0][2] - m_vertices[0][1] * m_vertices[2][2]))
		- (m_vertices[2][0] * (m_vertices[0][1] * m_vertices[1][2] - m_vertices[1][1] * m_vertices[0][2]));
	
}

bool rt_rectangle::inside_polygon(vector<float> pt)
{
	vector<float> v3 = m_vertices[3] - m_vertices[2];
	vector<float> v4 = pt - m_vertices[0];
	vector<float> v5 = m_vertices[2] - pt;
	vector<float> v1 = m_u_vec / m_u_size;
	v3 = vector_util::normalize(v3);
	v4 = vector_util::normalize(v4);
	v5 = vector_util::normalize(v5);
	float v1v4 = vector_util::dot(v1, v4);
	float v3v5 = vector_util::dot(v3, v5);
	return v1v4 > 0 && v3v5 > 0;
		

}

bool rt_rectangle::intersect(ray& ray, intersection_record& record)
{
	float dist = 0;
	vector<float> hitPt, n;

	n = m_normal;    // because ray/plane intersection may flip the normal!
	if (!ray_plane_intersection(ray, n, ma,mb,mc,md, &dist))
		return false;



	hitPt = ray.get_origin() + (ray.get_direction() * dist);

	/*
	* Now need to decide inside or outside
	*/
	if (!inside_polygon(hitPt))
		return false;

	record.update_record(dist, hitPt, n, ray, m_material_index, get_resource_index());
	return true;

}

void rt_rectangle::get_uv(vector<float> pt, vector<float> bc, float& u, float& v)
{
	//stubbed
	u = v = 0;
}

vector<float> rt_rectangle::get_position(float u, float v)
{
	return vector<float>();
}

vector<float> rt_rectangle::get_center()
{
	//average of all vertices
	vector<float> c = m_vertices[0];
	for (int i = 1; i < 4; i++)
	{
		c =c + m_vertices[i];
	}
	return c / 4;
}

float rt_rectangle::get_u_size()
{
	return m_u_size;
}

float rt_rectangle::get_v_size()
{
	return m_v_size;
}

vector<float> rt_rectangle::get_normal()
{
	return m_normal;
}

vector<float> rt_rectangle::get_max()
{
	//stub
	return vector<float>();
}

vector<float> rt_rectangle::get_min()
{
	//stub
	return vector<float>();
}