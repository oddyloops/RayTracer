
#include "rt_sphere.h"
#include "vector_util.h"

using namespace rt_support::geometries;

rt_sphere::rt_sphere(vector<float> center, float radius)
{
	m_center = center;
	m_radius = radius;
	m_radius_squared = radius * radius;
}

bool rt_sphere::intersect(ray& ray, intersection_record& record)
{
	vector<float> v1 = ray.get_origin() - m_center;
	float b = 2 * vector_util::dot(v1, ray.get_direction());
	float c = vector_util::magnitude_sqr(v1) - m_radius_squared;
	float root = b * b - 4 * c;

	if (root < 0)
		return false;

	root = sqrtf(root);
	float t0 = 0.5f * (-b - root);
	float t1 = 0.5f * (-b + root);

	if ((t0 < 0) && (t1 < 0))
		return false;

	float dist;
	if (t0 < t1)
	{
		if (t0 > 0)
			dist = t0;
		else
			dist = t1;
	}
	else
	{
		if (t1 > 0)
			dist = t1;
		else
			dist = t0;
	}

	if (dist > record.get_hit_distance())
		return false;

	// intersection found
	vector<float> pt = ray.get_origin() + dist * ray.get_direction();
	vector<float> n = pt - m_center;
	record.update_record(dist, pt, n, ray, m_material_index, get_resource_index());

	return true;

}

void rt_sphere::get_uv(vector<float> pt, vector<float> bc, float& u, float& v)
{
	//stubbed
	u = v = 0;
}

vector<float> rt_sphere::get_position(float u, float v)
{
	return vector<float>();
}

vector<float> rt_sphere::get_center()
{
	return m_center;
}

float rt_sphere::get_radius()
{
	return m_radius;
}


vector<float> rt_sphere::get_max()
{
	//stub
	return vector<float>();
}

vector<float> rt_sphere::get_min()
{
	//stub
	return vector<float>();
}