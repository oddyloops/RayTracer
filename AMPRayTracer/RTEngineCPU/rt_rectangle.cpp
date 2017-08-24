#include <math.h>
#include "rt_rectangle.h"
#include "math_util.h"
#include "matrix.h"
#include "vector_util.h"


using namespace rt_support::geometries;

rt_rectangle::rt_rectangle()
{

}


rt_rectangle& rt_rectangle::operator=(const rt_rectangle& r)
{
	m_type = r.m_type;
	m_material_index = r.m_material_index;
	m_normal = r.m_normal;
	m_type = r.m_type;
	m_u_axis_index = r.m_u_axis_index;
	m_u_size = r.m_u_size;
	m_u_vec = r.m_u_vec;
	int i = 0;
	for (vector<float> v : r.m_vertices) {
		m_vertices[i++] = v;
	}
	m_v_axis_index = r.m_v_axis_index;
	m_v_size = r.m_v_size;
	m_v_vec = r.m_v_vec;

	return *this;

}

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
	
	vector<float> v1, v2;
	m_u_vec = v1 = m_vertices[1] - m_vertices[0]; //horizontal lower corner - origin
	m_v_vec = v2 = m_vertices[3] - m_vertices[0];
	m_u_size = vector_util::magnitude(m_u_vec);
	m_v_size = vector_util::magnitude(m_v_vec);
	m_u_vec = vector_util::normalize(m_u_vec);
	m_v_vec = vector_util::normalize(m_v_vec);

	m_normal = vector_util::cross(v1, v2);
	m_normal = vector_util::normalize(m_normal);
	ma = m_normal[0];
	mb = m_normal[1];
	mc = m_normal[2];

	md = -vector_util::dot(m_normal, m_vertices[0]);
	
	if (math_util::abs(m_normal[0]) > math_util::abs(m_normal[1]))
	{
		/*normal x > normal y*/
		if (math_util::abs(m_normal[0]) > math_util::abs(m_normal[2]))
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
		if (math_util::abs(m_normal[1]) > math_util::abs(m_normal[2]))
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

bool rt_rectangle::inside_polygon(vector<float> pt)
{
	vector<float> v3 = m_vertices[3] - m_vertices[2];
	vector<float> v4 = pt - m_vertices[0];
	vector<float> v5 = pt - m_vertices[2];
	vector<float> v1 = m_u_vec;
	v3 = vector_util::normalize(v3);
	v4 = vector_util::normalize(v4);
	v5 = vector_util::normalize(v5);

	//compute clockwise angle between v1 and v4 : v3 and v5

	float v1v4 = clock_wise_angle(v1, v4, m_normal);
	float v3v5 = clock_wise_angle(v3, v5, m_normal);
	float angle90 = 0.5f * PI;
	return (v1v4 < angle90 && v1v4 > 0 ) && (v3v5 < angle90 && v3v5 > 0);
	
}

float rt_rectangle::clock_wise_angle(vector<float> v1, vector<float> v2, vector<float> normal)
{
	float dot = vector_util::dot(v1, v2);
	float det = v1[0] * v2[1] * normal[2]
		+ v2[0] * normal[1] * v1[2]
		+ normal[0] * v1[1] * v2[2]
		- v1[2] * v2[1] * normal[0]
		- v2[2] * normal[1] * v1[0]
		- normal[2] * v1[1] * v2[0];
	return atan2f(det, dot);
}

bool rt_rectangle::intersect(ray& ray, intersection_record& record)
{
	float dist = 0;
	vector<float> hitPt, n;

	n = m_normal;    // because ray/plane intersection may flip the normal!
	if (!ray_plane_intersection(ray, n,md,dist,m_vertices[0]))
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


vector<float> rt_rectangle::get_vertex(int i)
{
	return m_vertices[i];
}