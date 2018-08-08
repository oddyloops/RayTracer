
#include "rt_sphere.h"
#include "vector_util.h"

using namespace rt_support::geometries;

rt_sphere::rt_sphere(vector<float> center, float radius)
{
	m_center = center;
	m_radius = radius;
	m_radius_squared = radius * radius;
	m_type = rt_geometry_type::sphere;
}

bool rt_sphere::intersect(ray& ray, intersection_record& record)
{

	vector<float> l = m_center - ray.get_origin();
	float l_size = vector_util::magnitude(l);
	//project l on the ray direction
	float ca = vector_util::dot(l, ray.get_direction());
	if (ca < 0)
		return false;
	float d = sqrtf(l_size * l_size - ca * ca);
	if (d < 0 || d > m_radius)
		return false;

	//compute ray entry point
	float hc = sqrtf(m_radius * m_radius - d * d);
	vector<float> pt = ray.get_origin() + (ca - hc)*ray.get_direction(); //adding ca and hc will give exit intersection
	vector<float> n = vector_util::normalize(pt - m_center); //normal at entry intersection
	record.update_record(ca-hc, pt, n, ray, m_material_index, get_resource_index(),m_type);

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