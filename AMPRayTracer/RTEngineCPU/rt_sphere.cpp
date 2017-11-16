
#include "rt_sphere.h"
#include "vector_util.h"

using namespace rt_support::geometries;

rt_sphere::rt_sphere(vector<float> center, float radius)
{
	m_center = center;
	m_radius = radius;
	m_radius_squared = radius * radius;
	m_vertical_axis = { 0,1,0 }; // axis aligned by default
	m_hor_axis_1 = { 1,0,0 };
	m_hor_axis_2 = { 0,0,1 };
	m_base_point = m_center - (m_radius * m_vertical_axis);


	m_type = rt_geometry_type::sphere;

}


rt_sphere::rt_sphere(vector<float> center, float radius, vector<float> vertical_axis)
{
	m_center = center;
	m_radius = radius;
	m_radius_squared = radius * radius;
	m_type = rt_geometry_type::sphere;

	m_vertical_axis = vector_util::normalize(vertical_axis); 
	m_base_point = m_center - (m_radius * m_vertical_axis);

	m_hor_axis_1 = vector_util::normalize(vector_util::cross(m_vertical_axis, { 0,1,0 }));
	if (vector_util::is_zero(m_hor_axis_1))
	{
		m_hor_axis_1 = { 1,0,0 };
		m_hor_axis_2 = { 0,0,1 };
	}
	else {
		m_hor_axis_2 = vector_util::normalize(vector_util::cross(m_hor_axis_1, m_vertical_axis));
	}

	

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
	float dist = ca - hc;
	vector<float> pt = ray.get_origin() +dist*ray.get_direction(); //adding ca and hc will give exit intersection
	vector<float> n = vector_util::normalize(pt - m_center); //normal at entry intersection
	
	
	float u, v = 0;
	get_uv(pt,{ 0 }, u, v);
	vector<float> normal = n;
	if (!m_normal_map.is_null())
	{

		normal = vector_util::normalize(get_normal(u, v));


	}

	if (!m_bump_map.is_null())
	{
		//bump the point by a height b along the normal
		float b = m_bump_map.get_value(u, v);
		vector<float> old_hit_point = pt;
		pt = old_hit_point + b * normal;

		float dist_diff = vector_util::magnitude(pt - old_hit_point);
		dist += dist_diff;
	}

	record.update_record(dist, pt, n, ray, m_material_index, get_resource_index(),m_type,u,v);

	return true;

}

void rt_sphere::get_uv(vector<float> pt, vector<float> bc, float& u, float& v)
{
	vector<float> pt_base = pt - m_base_point;
	vector<float> axis_point = m_base_point + (vector_util::dot(pt_base, m_vertical_axis) * m_vertical_axis);
	vector<float> p = pt - axis_point;
	u = atan2(vector_util::dot(p,m_hor_axis_1), vector_util::dot(p,m_hor_axis_2)) / (2 * PI) + 0.5f; //https://gamedev.stackexchange.com/questions/114412/how-to-get-uv-coordinates-for-sphere-cylindrical-projection
	v = vector_util::dot(pt_base, m_vertical_axis) / (2 * m_radius);
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


