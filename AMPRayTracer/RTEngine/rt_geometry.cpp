#include <amp_math.h>
#include <cfloat>
#include "rt_geometry.h"
#include "vector_amp.h"
#include "matrix_amp.h"
#include "math_util.h"

#include "rt_sphere.h"
#include "rt_cylinder.h"
#include "rt_rectangle.h"
#include "rt_plane.h"
#include "rt_triangle.h"



using namespace concurrency::fast_math;
using namespace rt_support::geometries;


rt_geometry::rt_geometry()
{

}


rt_geometry::rt_geometry(int geom_type)
{
	if (geom_type  < rt_geometry_type::sphere || geom_type > rt_geometry_type::cylinder)
	{
		throw std::exception("Invalid geometry type");
	}
	m_type = geom_type;
}

void rt_geometry::construct_sphere(float radius, float_3 center)
{
	construct_sphere(radius, center, float_3(0, 1, 0));
}


void rt_geometry::construct_sphere(float radius, float_3 center, float_3 axis_dir)
{
	if (m_type != rt_geometry_type::sphere)
	{
		throw std::exception("Geometry was not defined as a sphere!");
	}
	m_center = center;
	m_radius = radius;
	m_radius_sq = radius * radius;


	m_axis_dir = vector_amp::normalize(axis_dir);
	m_base_center = m_center - (m_radius * axis_dir);

	m_hor_axis_dir = vector_amp::normalize(vector_amp::cross(m_axis_dir, float_3(0, 1, 0)));
	if (vector_amp::is_zero(m_hor_axis_dir))
	{
		m_hor_axis_dir = float_3(1, 0, 0);
		m_hor_axis_dir_perp = float_3(0, 0, 1);
	}
	else {
		m_hor_axis_dir_perp = vector_amp::normalize(vector_amp::cross(m_hor_axis_dir, m_axis_dir));
	}

}


void rt_geometry::construct_cylinder(float radius, float_3 top_center, float_3 base_center)
{

	if (m_type != rt_geometry_type::cylinder)
	{
		throw std::exception("Geometry was not defined as a cylinder!");
	}
	m_radius = radius;
	m_radius_sq = m_radius * m_radius;
	m_top_center = top_center;
	m_base_center = base_center;
	m_height = vector_amp::magnitude(top_center - base_center);
	m_axis_dir = (top_center - base_center) / m_height;
	m_orth_d = -vector_amp::dot(m_axis_dir, m_base_center);
	m_curve_section = m_height / (2 * m_radius + m_height);
	m_flat_section = 0.5f * (1 - m_curve_section);

	//approximate horizontal axes directions by crossing vertical axis with y axis
	m_hor_axis_dir = vector_amp::normalize(vector_amp::cross(m_axis_dir, { 0,1,0 }));

	if (vector_amp::is_zero(m_hor_axis_dir) == true)
	{
		m_hor_axis_dir = float_3(1, 0, 0); //x axis
		m_hor_axis_dir_perp = float_3(0, 0, 1); //z axis
	}
	else {
		m_hor_axis_dir_perp = vector_amp::normalize(vector_amp::cross(m_hor_axis_dir, m_axis_dir));
	}
}


void  rt_geometry::construct_rectangle(float_3 vertices[])
{
	if (m_type != rt_geometry_type::rectangle)
	{
		throw std::exception("Geometry was not defined as a rectangle!");
	}

	if (vertices + 3 == nullptr) //not up to four vertices?
	{
		throw std::exception("Rectangle must have 4 vertices!");
	}

	for (int i = 0; i < 4; i++)
	{
		m_vertices[i] = vertices[i];
	}

	initialize_rectangle();

}


void  rt_geometry::construct_rectangle(float_3 vertices[], concurrency::array<float, 2> xform)
{
	if (m_type != rt_geometry_type::rectangle)
	{
		throw std::exception("Geometry was not defined as a rectangle!");
	}

	if (vertices + 3 == nullptr) //not up to four vertices?
	{
		throw std::exception("Rectangle must have 4 vertices!");
	}

	for (int i = 0; i < 4; i++)
	{
		m_vertices[i] = vertices[i];
	}

	
	for (int i = 0; i < 4; i++)
	{
		m_vertices[i] = matrix_amp::transform(m_vertices[i], xform);
	}

	initialize_rectangle();
}


void rt_geometry::initialize_rectangle()
{
	float_3 v1, v2;
	m_u_vec = v1 = m_vertices[1] - m_vertices[0]; //horizontal lower corner - origin
	m_v_vec = v2 = m_vertices[3] - m_vertices[0];
	m_u_size = vector_amp::magnitude(m_u_vec);
	m_v_size = vector_amp::magnitude(m_v_vec);
	m_u_vec = vector_amp::normalize(m_u_vec);
	m_v_vec = vector_amp::normalize(m_v_vec);

	m_true_normal = vector_amp::cross(v1, v2);
	m_true_normal = vector_amp::normalize(m_true_normal);
	ma = m_true_normal.x;
	mb = m_true_normal.y;
	mc = m_true_normal.z;

	md = -vector_amp::dot(m_true_normal, m_vertices[0]);

	if (math_util::abs(m_true_normal.x) > math_util::abs(m_true_normal.y))
	{
		/*normal x > normal y*/
		if (math_util::abs(m_true_normal.x) > math_util::abs(m_true_normal.z))
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
		if (math_util::abs(m_true_normal.y) > math_util::abs(m_true_normal.z))
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



void rt_geometry::construct_plane(float_3 points[], float map_width, float map_height)
{
	if (m_type != rt_geometry_type::plane)
	{
		throw std::exception("Geometry was not defined as a plane!");
	}


	if (points + 2 == nullptr)
	{
		throw std::exception("Plane must have at least 3 non co-linear points");
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


void rt_geometry::construct_triangle(float_3 vertices[])
{

	if (m_type != rt_geometry_type::triangle)
	{
		throw std::exception("Geometry was not defined as a triangle!");
	}

	if (vertices + 2 == nullptr)
	{
		//not up to three vertices?
		throw std::exception("Triangle must have 3 vertices!");
	}

	m_type = rt_geometry_type::triangle;
	for (int i = 0; i < 3; i++)
	{
		m_vertices[i] = vertices[i];
	}
	initialize_triangle();
}


void rt_geometry::construct_triangle(float_3 vertices[], concurrency::array<float,2> xform)
{

	if (m_type != rt_geometry_type::triangle)
	{
		throw std::exception("Geometry was not defined as a triangle!");
	}

	if (vertices + 2 == nullptr)
	{
		//not up to three vertices?
		throw std::exception("Triangle must have 3 vertices!");
	}

	for (float_3* v = vertices; v != nullptr; v++)
	{
		*v = matrix_amp::transform(*v, xform);
	}

	m_type = rt_geometry_type::triangle;
	for (int i = 0; i < 3; i++)
	{
		m_vertices[i] = vertices[i];
	}
	initialize_triangle();
}


void rt_geometry::initialize_triangle()
{
	m_u_vec = m_vertices[1] - m_vertices[0];
	m_v_vec = m_vertices[2] - m_vertices[0];
	m_true_normal = vector_amp::normalize(vector_amp::cross(m_u_vec, m_v_vec));
	md = -vector_amp::dot(m_true_normal, m_vertices[0]);
	m_apex_u = vector_amp::dot(m_v_vec, vector_amp::normalize(m_u_vec)) / vector_amp::magnitude(m_v_vec);
}

void rt_geometry::set_material_index(int index)
{
	m_material_index = index;
}




int rt_geometry::intersect(ray& r, intersection_record& record) restrict(amp)
{
	
	int intersects = false;
	float dist;
	float_3 pt, n;
	int dist_index = -1;
	switch (m_type)
	{
	case rt_geometry_type::sphere:
		intersects = rt_sphere::intersect(r, record, dist, pt, n, m_center, m_radius);
		break;
	case rt_geometry_type::cylinder:
		
		intersects = rt_cylinder::intersect(r, record,dist,pt,n, dist_index, m_height, m_axis_dir, m_base_center,
			m_top_center, m_radius_sq, m_orth_d);
		break;
	case rt_geometry_type::rectangle:
		intersects = rt_rectangle::intersect(r, record, dist, pt, n, m_true_normal, m_vertices, md, m_u_vec);
		break;
	case rt_geometry_type::plane:
		intersects = rt_plane::intersect(r, record, pt, dist,n, m_true_normal, point_0, md);
		break;
	case rt_geometry_type::triangle:
		intersects = rt_triangle::intersect(r, record, pt, dist, n, m_true_normal, m_vertices, m_u_vec, m_v_vec, md);
		
		break;
	}
	


	if (intersects == true)
	{
		record.update_record(dist, pt, n, r, m_material_index, get_resource_index(), m_type);
	}
	return intersects;
}

int rt_geometry::intersect(ray& r, intersection_record& record, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars, 
	texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp)
{
	int intersects = intersect(r,record);
	

	if (intersects == true)
	{
		float dist = record.get_hit_distance();
		float_3 pt = record.get_intersection_position();
		float u, v;
		float_3 normal = record.get_normal_at_intersect();
		float_3 bc;
		get_uv(pt, bc, u, v);
		
		if (!m_normal_map.is_null())
		{
			normal = vector_amp::normalize(get_normal(u, v, bitmaps, scalars));
		}

		if (!m_bump_map.is_null())
		{
			//bump the point by a height b along the normal
			float b = m_bump_map.get_value(u, v, f_bitmaps, f_scalars);
			float_3 old_hit_point = pt;
			pt = old_hit_point + b * normal;
			float dist_diff = vector_amp::magnitude(pt - old_hit_point);
			dist += dist_diff;
		}


		record.force_update_record(dist, pt, normal, r, m_material_index, get_resource_index(), m_type);
		
	}
	return intersects;
}


float_3 rt_geometry::get_bc(float_3 pt) restrict(amp)
{
	float_3 p0 = pt - m_vertices[0];
	float_3 p1 = pt - m_vertices[1];
	float_3 p2 = pt - m_vertices[2];
	float a = vector_amp::magnitude(vector_amp::cross(p1 - p0, p2 - p0));
	float a1 = vector_amp::magnitude(vector_amp::cross(p2, p1)) / a;
	float a2 = vector_amp::magnitude(vector_amp::cross(p1, p0)) / a;
	float a3 = vector_amp::magnitude(vector_amp::cross(p0, p2)) / a;
	return float_3(a1, a3, a2);
}

float_3 rt_geometry::get_min() restrict(amp,cpu)
{
	return m_min;
}

float_3 rt_geometry::get_max() restrict(amp,cpu)
{
	return m_max;
}

void rt_geometry::get_uv(float_3 pt, float_3 bc, float& u, float& v, int dist_index) restrict(amp)
{

	switch (m_type)
	{
	case rt_geometry_type::sphere:
		rt_sphere::get_uv(pt, bc, u, v, m_radius, m_base_center, m_axis_dir, m_hor_axis_dir, m_hor_axis_dir_perp);
		break;
	case rt_geometry_type::cylinder:
		rt_cylinder::get_uv(pt, bc, u, v, dist_index, m_base_center, m_top_center, m_axis_dir, m_hor_axis_dir, m_hor_axis_dir_perp, m_curve_section,
			m_flat_section, m_height, m_radius);
		break;
	case rt_geometry_type::rectangle:
		rt_rectangle::get_uv(pt, bc, u, v, m_vertices[0], m_u_vec, m_v_vec, m_u_size, m_v_size);
		break;
	case rt_geometry_type::plane:
		bc = get_bc(pt);
		rt_plane::get_uv(pt, bc, u, v, point_0, m_u_vec, m_v_vec, m_map_width, m_map_height);
		break;
	case rt_geometry_type::triangle:
		rt_triangle::get_uv(pt, bc, u, v, m_apex_u);
		break;
	}

}

float_3 rt_geometry::get_position(float u, float v) restrict(amp,cpu)
{
	//stub since virtual methods are not allowed
	float_3 blank;
	return blank;
}

int rt_geometry::get_type() restrict(amp,cpu)
{
	return m_type;
}

int rt_geometry::get_material_index() restrict(amp,cpu)
{
	return m_material_index;
}

array<float,2> rt_geometry::parse_xform(float_3 translation, float rx, float ry, float rz, float_3 scale)
{
	
	float init[] = { 0,0,0,0,0,0,0,0,0 };
	array<float, 2> rotation(3, 3, init);
	if(rx > FLT_EPSILON)
		rotation = rotation * matrix_amp::create_rotation_x(math_util::deg_to_rad(rx));
	if(ry > FLT_EPSILON)
		rotation = rotation * matrix_amp::create_rotation_y(math_util::deg_to_rad(ry));
	if (rz > FLT_EPSILON)
		rotation = rotation * matrix_amp::create_rotation_z(math_util::deg_to_rad(rz));
	return matrix_amp::create_scale_from_vector(scale) * rotation * matrix_amp::create_translation_from_vector(translation);

}

void rt_geometry::set_normal_map(texture_map<float_3> normal_map) restrict(amp, cpu)
{
	m_normal_map = normal_map;
}

void rt_geometry::set_bump_map(texture_map<float> bump_map) restrict(amp, cpu)
{
	m_bump_map = bump_map;
}

float_3 rt_geometry::get_normal(float u, float v, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars) restrict(amp)
{
	return m_normal_map.get_value(u, v, bitmaps, scalars);
}