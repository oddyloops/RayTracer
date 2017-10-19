#include "rt_plane.h"
#include "vector_amp.h"

using namespace rt_support::geometries;

rt_plane::rt_plane() restrict(amp,cpu) {}

rt_plane::rt_plane(float_3 points[]) restrict(amp,cpu)
{
	if (points + 2 == nullptr)
	{
		return;
	}
	m_type = rt_geometry_type::plane;
	point_0 = points[0];
	float_3 v1 = points[1] - points[0];
	float_3 v2 = points[2] - points[0];

	m_normal = vector_amp::normalize(vector_amp::cross(v1, v2));
	md = -vector_amp::dot(v1, m_normal);
}

int rt_plane::intersect(ray& ray, intersection_record& record) restrict(amp)
{
	float_3 n = m_normal;
	float dist = 0;
	int intersects = ray_plane_intersection(ray, n, md, dist, point_0);
	float_3 hitPt = ray.get_origin() + dist * ray.get_direction();
	if (intersects)
	{
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


float_3 rt_plane::get_normal() restrict(amp)
{
	return m_normal;
}
