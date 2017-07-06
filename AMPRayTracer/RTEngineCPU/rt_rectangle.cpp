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
	vector<float> v1, v2;
	m_u_vec = v1 = m_vertices[1] - m_vertices[0]; //horizontal lower corner - origin
	m_v_vec = v2 = m_vertices[3] - m_vertices[0];
	m_u_size = vector_util::magnitude(m_u_vec);
	m_v_size = vector_util::magnitude(m_v_vec);
	m_u_vec = vector_util::normalize(m_u_vec);
	m_v_vec = vector_util::normalize(m_v_vec);

	m_normal = vector_util::cross(v1, v2);
	m_normal = vector_util::normalize(m_normal);
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

int rt_rectangle::inside_polygon(vector<float> pt)
{
	float va[3];
	float vb[3];
	float trans_vector[3];
	int NC = 0; /*number of crossings*/
	int NSH, SH; /*sign holder*/
	int b;
	float u_intersect;
	trans_vector[0] = -pt[0];
	trans_vector[1] = -pt[1];
	trans_vector[2] = -pt[2];

	va[0] = m_vertices[0][0] + trans_vector[0];
	va[1] = m_vertices[0][1] + trans_vector[1];
	va[2] = m_vertices[0][2] + trans_vector[2];

	if (va[m_v_axis_index] < 0)
	{
		SH = -1;
	}
	else
	{
		SH = 1;
	}

	for (int i = 0; i < 4; i++)
	{
		b = (i + 4) % 4;
		vb[0] = m_vertices[b][0] + trans_vector[0];
		vb[1] = m_vertices[b][1] + trans_vector[0];
		vb[2] = m_vertices[b][2] + trans_vector[0];

		if (vb[m_v_axis_index] < 0)
		{
			NSH = -1;
		}
		else 
		{
			NSH = 1;
		}
		
		if (SH != NSH)
		{
			if ((va[m_u_axis_index] > 0) && (vb[m_u_axis_index] > 0))
			{
				/*
				* Line crossed +U
				*/
				NC++;
			}
			else
			{
				if ((va[m_u_axis_index] > 0) || (vb[m_u_axis_index] > 0))
				{
					//line might cross +U, so compute U intersectoin
					u_intersect = va[m_u_axis_index] - (va[m_v_axis_index] *
						(vb[m_u_axis_index] - va[m_u_axis_index]) /
						(vb[m_v_axis_index] - va[m_v_axis_index]));
					if (u_intersect > 0)
					{
						// Line crossed +U
						NC++;
					}
				}
			}
			SH = NSH;
			va[0] = vb[0];
			va[1] = vb[1];
			va[2] = vb[2];

		}
		
	}
	return NC % 2 != 0;
}

bool rt_rectangle::intersect(ray& ray, intersection_record& record)
{
	float dist = 0;
	vector<float> hitPt, n;

	n = m_normal;    // because ray/plane intersection may flip the normal!
	if (!ray_plane_intersection(ray, n, md, dist))
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