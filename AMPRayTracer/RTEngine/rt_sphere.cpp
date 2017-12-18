
#include <amp_math.h>
#include "rt_sphere.h"
#include "vector_amp.h"

using namespace concurrency::fast_math;
using namespace rt_support::geometries;

rt_sphere::rt_sphere(float_3 center, float radius)
{
	m_center = center;
	m_radius = radius;
	m_radius_squared = m_radius * m_radius;

	m_vertical_axis = float_3( 0,1,0 ); // axis aligned by default
	m_hor_axis_1 = float_3( 1,0,0 );
	m_hor_axis_2 = float_3( 0,0,1 );
	m_base_point = m_center - (m_radius * m_vertical_axis);
	m_type = rt_geometry_type::sphere;
}

rt_sphere::rt_sphere(float_3 center, float radius, float_3 vertical_axis)
{
	m_center = center;
	m_radius = radius;
	m_radius_squared = radius * radius;
	m_type = rt_geometry_type::sphere;

	m_vertical_axis = vector_amp::normalize(vertical_axis);
	m_base_point = m_center - (m_radius * m_vertical_axis);

	m_hor_axis_1 = vector_amp::normalize(vector_amp::cross(m_vertical_axis, float_3( 0,1,0 )));
	if (vector_amp::is_zero(m_hor_axis_1))
	{
		m_hor_axis_1 = float_3( 1,0,0 );
		m_hor_axis_2 = float_3( 0,0,1 );
	}
	else {
		m_hor_axis_2 = vector_amp::normalize(vector_amp::cross(m_hor_axis_1, m_vertical_axis));
	}



}




int rt_sphere::intersect(ray& r, intersection_record& record) restrict(amp)
{
	float_3 l = m_center - r.get_origin();
	float l_size = vector_amp::magnitude(l);
	//project l on the ray direction
	float ca = vector_amp::dot(l, r.get_direction());
	if (ca < 0)
		return false;
	float d = sqrtf(l_size * l_size - ca * ca);
	if (d < 0 || d > m_radius)
		return false;

	//compute ray entry point
	float hc = sqrtf(m_radius * m_radius - d * d);
	float_3 pt = r.get_origin() + (ca - hc)*r.get_direction(); //adding ca and hc will give exit intersection
	float_3 n = vector_amp::normalize(pt - m_center); //normal at entry intersection
	float dist = ca - hc;

	record.update_record(dist, pt, n, r, m_material_index, get_resource_index(), m_type);
	return true;

}


int rt_sphere::intersect(ray& r, intersection_record& record, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
	, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp)
{
	if (intersect(r, record) == false)
	{
		return false;
	}
	float_3 pt = record.get_intersection_position();
	float_3 n = record.get_normal_at_intersect();
	float dist = record.get_hit_distance();

	float u, v = 0;
	get_uv(pt, { 0 }, u, v);
	float_3 normal = n;

	if (!m_normal_map.is_null())
	{

		normal = vector_amp::normalize(get_normal(u, v, bitmaps,scalars));


	}

	if (!m_bump_map.is_null())
	{
		//bump the point by a height b along the normal
		float b = m_bump_map.get_value(u, v, f_bitmaps,f_scalars);
		float_3 old_hit_point = pt;
		pt = old_hit_point + b * normal;

		float dist_diff = vector_amp::magnitude(pt - old_hit_point);
		dist += dist_diff;
	}

	record.force_update_record(dist, pt, n, r, m_material_index, get_resource_index(),m_type);
	return true;

}

void rt_sphere::get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp)
{
	const float PI = 3.14159F;
	float_3 pt_base = pt - m_base_point;
	float_3 axis_point = m_base_point + (vector_amp::dot(pt_base, m_vertical_axis) * m_vertical_axis);
	float_3 p = pt - axis_point;
	u = (atan2(vector_amp::dot(p, m_hor_axis_1), vector_amp::dot(p, m_hor_axis_2)) + PI) / (2 * PI); //https://gamedev.stackexchange.com/questions/114412/how-to-get-uv-coordinates-for-sphere-cylindrical-projection
	v = vector_amp::dot(pt_base, m_vertical_axis) / (2 * m_radius);
}

float_3 rt_sphere::get_position(float u, float v) restrict(amp)
{
	return float_3();
}

float_3 rt_sphere::get_center() restrict(amp)
{
	return m_center;
}

float rt_sphere::get_radius() restrict(amp)
{
	return m_radius;
}


float_3 rt_sphere::get_max() restrict(amp)
{
	//stub
	return float_3();
}

float_3 rt_sphere::get_min() restrict(amp)
{
	//stub
	return float_3();
}