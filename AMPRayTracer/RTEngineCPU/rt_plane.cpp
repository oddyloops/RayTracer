#include "rt_plane.h"
#include "vector_util.h"

using namespace rt_support::geometries;

rt_plane::rt_plane() {}

rt_plane::rt_plane(vector<float> points[], float map_width, float map_height)
{
	if (points + 2 == nullptr)
	{
		throw exception("You need three points to define a 3D plane");
	}
	m_type = rt_geometry_type::plane;
	point_0 = points[0];
	vector<float> v1 = points[1] - points[0];
	vector<float> v2 = points[2] - points[0];

	m_true_normal = vector_util::normalize(vector_util::cross(v1, v2));
	m_u_vec = vector_util::normalize(v1);
	m_v_vec = vector_util::normalize(v2);
	m_map_width = map_width;
	m_map_height = map_height;

	md = -vector_util::dot(v1, m_true_normal);
}

bool rt_plane::intersect(ray& ray, intersection_record& record)
{
	vector<float> n = m_true_normal;
	float dist = 0;
	bool intersects = ray_plane_intersection(ray, n, md, dist, point_0);
	vector<float> hitPt = ray.get_origin() + dist * ray.get_direction();
	if (intersects)
	{
		vector<float> normal = m_true_normal;
		float u, v;
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
		
	}
	return intersects;
}


void rt_plane::get_uv(vector<float> pt, vector<float> bc, float& u, float& v)
{
	vector<float> p = pt - point_0;
	//project vector p along u and v
	float u_dist = vector_util::dot(p, m_u_vec);
	float v_dist = vector_util::dot(p, m_v_vec); 
	
	int u_mult = static_cast<int>(abs(u_dist) / m_map_width);
	int v_mult = static_cast<int>(abs(v_dist) / m_map_height);
	float u_rem = abs(u_dist) - u_mult * m_map_width;
	float v_rem = abs(v_dist) - v_mult * m_map_height; //used for tiling based on the set map width and height
	u_rem /= m_map_width;
	v_rem /= m_map_height;
	if (u_dist < 0)
	{
		u_rem = 1 - u_rem;
	}

	if (v_dist < 0)
	{
		v_rem = 1 - v_rem;
	}

	u = u_rem;
	v = v_rem;
}


vector<float> rt_plane::get_position(float u, float v)
{
	return { 0 };
}


