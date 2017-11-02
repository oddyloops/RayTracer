#include <amp_math.h>
#include <cfloat>
#include "rt_geometry.h"
#include "vector_util.h"
#include "math_util.h"

using namespace concurrency::fast_math;
using namespace rt_support::geometries;


void rt_geometry::set_material_index(int index)
{
	m_material_index = index;
}


void rt_geometry::set_normal_map(vector_map n_map)
{
	m_normal_map = n_map;
}

void rt_geometry::set_bump_map(float_map b_map)
{
	m_bump_map = b_map;
}

float rt_geometry::plane_point_dist(vector<float> pt,vector<float> norm, vector<float> plane_point)
{
	vector<float> v = pt - plane_point;
	return vector_util::dot(v, norm);
}

bool rt_geometry::ray_plane_intersection(ray& r, vector<float>& norm, float d, float& dist, vector<float> plane_point)
{

	float denomenator = vector_util::dot(norm, r.get_direction());

	/*
	* Ray perpendicular to normal
	*/
	if (fabs(denomenator) < FLT_EPSILON)
		return false;

	float numerator = vector_util::dot(norm, (plane_point - r.get_origin()));
	dist = numerator / denomenator;

	if (dist < 0)
	{
		return false;
	}

	if (denomenator > 0)
		norm = -1 * norm;

	return true;


}

bool rt_geometry::intersect(ray& r, intersection_record& record)
{
	//stub since virtual methods are not allowed
	return false;
}

vector<float> rt_geometry::get_min()
{
	return m_min;
}

vector<float> rt_geometry::get_max()
{
	return m_max;
}

void rt_geometry::get_uv(vector<float> pt, vector<float> bc, float& u, float& v)
{
	//stub since virtual methods are not allowed
}

vector<float> rt_geometry::get_position(float u, float v)
{
	//stub since virtual methods are not allowed
	vector<float> blank;
	return blank;
}

int rt_geometry::get_type() 
{
	return m_type;
}

int rt_geometry::get_material_index()
{
	return m_material_index;
}

matrix<float> rt_geometry::parse_xform(vector<float> translation, float rx, float ry, float rz, vector<float> scale)
{
	
	matrix<float> init(3, 3, 0);

	if(abs(rx) > FLT_EPSILON)
		init = init * matrix<float>::rotate_x(math_util::deg_to_rad(rx));
	if(abs(ry) > FLT_EPSILON)
		init = init * matrix<float>::rotate_y(math_util::deg_to_rad(ry));
	if (abs(rz) > FLT_EPSILON)
		init = init * matrix<float>::rotate_z(math_util::deg_to_rad(rz));
	return matrix<float>::scale(scale) * init * matrix<float>::translate_from_vector(translation);

}


vector<float>  rt_geometry::get_normal(float u, float v)
{
	return m_normal_map.get_value(u, v);
}