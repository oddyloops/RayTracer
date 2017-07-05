#include <amp_math.h>
#include <cfloat>
#include "rt_geometry.h"
#include "vector_util.h"
#include "math_util.h"

using namespace concurrency::fast_math;
using namespace rt_support::geometries;

int rt_geometry::ray_plane_intersection(ray& r, vector<float> norm, float d, float& dist)
{
	float denomenator = vector_util::dot(norm, r.get_direction());

	/*
	* Ray perpendicular to normal
	*/
	if (fabs(denomenator) < FLT_EPSILON)
		return false;

	float NdotO = vector_util::dot(norm, r.get_origin());
	dist = -(NdotO + d) / denomenator;

	if (denomenator > 0)
		norm =  vector_util::negate(norm);

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

matrix rt_geometry::parse_xform(vector<float> translation, float rx, float ry, float rz, vector<float> scale)
{
	
	matrix init(3, 3, 0);

	if(rx > FLT_EPSILON)
		init = init * matrix::rotate_x(math_util::deg_to_rad(rx));
	if(ry > FLT_EPSILON)
		init = init * matrix::rotate_y(math_util::deg_to_rad(ry));
	if (rz > FLT_EPSILON)
		init = init * matrix::rotate_z(math_util::deg_to_rad(rz));
	return matrix::scale(scale) * init * matrix::translate_from_vector(translation);

}