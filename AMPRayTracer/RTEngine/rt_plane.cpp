#include "rt_plane.h"
#include "vector_amp.h"

using namespace rt_support::geometries;

rt_plane::rt_plane() restrict(amp,cpu) {}

rt_plane::rt_plane(float_3 points[], float map_width, float map_height) restrict(amp,cpu)
{
	if (points + 2 == nullptr)
	{
		return;
	}
	m_type = rt_geometry_type::plane;
	point_0 = points[0];
	float_3 v1 = points[1] - points[0];
	float_3 v2 = points[2] - points[0];
	m_u_vec = vector_amp::normalize(v1);
	m_v_vec = vector_amp::normalize(v2);
	m_map_width = map_width;
	m_map_height = map_height;

	m_true_normal = vector_amp::normalize(vector_amp::cross(v1, v2));
	md = -vector_amp::dot(v1, m_true_normal);
}

int rt_plane::intersect(ray& ray, intersection_record& record, array_view<float_3, 3>* bitmaps, array_view<float_3, 1>* scalars
	, array_view<float, 3>* f_bitmaps, array_view<float, 1>* f_scalars) restrict(amp)
{
	float_3 n = m_true_normal;
	float dist = 0;
	int intersects = ray_plane_intersection(ray, n, md, dist, point_0);
	float_3 hitPt = ray.get_origin() + dist * ray.get_direction();
	if (intersects)
	{
		float_3 normal = m_true_normal;
		float u, v;
		get_uv(hitPt, { 0 }, u, v);
		if (!m_normal_map.is_null())
		{

			normal = vector_amp::normalize(get_normal(u, v, bitmaps, scalars));

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
		record.update_record(dist, hitPt, n, ray, m_material_index, get_resource_index(), m_type);
	}
	return intersects;
}


void rt_plane::get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp)
{

}


float_3 rt_plane::get_position(float u, float v) restrict(amp)
{
	return { 0 };
}

