#include "rt_plane.h"
#include "vector_util.h"

using namespace rt_support::geometries;

rt_plane::rt_plane() {}

rt_plane::rt_plane(vector<float> points[])
{
	if (points + 2 == nullptr)
	{
		throw exception("You need three points to define a 3D plane");
	}
	m_type = rt_geometry_type::plane;
	point_0 = points[0];
	vector<float> v1 = points[1] - points[0];
	vector<float> v2 = points[2] - points[0];

	m_normal = vector_util::normalize(vector_util::cross(v1, v2));
	md = -vector_util::dot(v1, m_normal);
}

bool rt_plane::intersect(ray& ray, intersection_record& record)
{
	vector<float> n = m_normal;
	float dist = 0;
	bool intersects = ray_plane_intersection(ray, n, md, dist, point_0);
	vector<float> hitPt = ray.get_origin() + dist * ray.get_direction();
	if (intersects)
	{
		record.update_record(dist, hitPt, n, ray, m_material_index, get_resource_index(), m_type);
	}
	return intersects;
}


void rt_plane::get_uv(vector<float> pt, vector<float> bc, float& u, float& v)
{

}


vector<float> rt_plane::get_position(float u, float v)
{
	return { 0 };
}


vector<float> rt_plane::get_normal()
{
	return m_normal;
}
