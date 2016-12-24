#include <amp_math.h>
#include <cfloat>
#include "rt_geometry.h"
#include "vector_amp.h"

using namespace concurrency::fast_math;
using namespace rt_support::geometries;

bool rt_geometry::ray_plane_intersection(ray& r, float_3 norm, float_3 d, float_3& dist) restrict(amp)
{
	float denomenator = vector_amp::dot(norm, r.get_direction());

	/*
	* Ray perpendicular to normal
	*/
	if (fabs(denomenator) < FLT_EPSILON)
		return false;

	float NdotO = vector_amp::dot(norm, r.get_origin());
	dist = -(NdotO + d) / denomenator;

	if (denomenator > 0)
		norm = -norm;

	return true;

}

bool rt_geometry::intersect(ray& r, const intersection_record& record) restrict(amp) restrict(cpu)
{
	//stub since virtual methods are not allowed
	return false;
}

float_3 rt_geometry::get_min() restrict(amp) restrict(cpu)
{
	return this->m_min;
}

float_3 rt_geometry::get_max() restrict(amp) restrict(cpu)
{
	return this->m_max;
}

void rt_geometry::get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp) restrict(cpu)
{
	//stub since virtual methods are not allowed
}

float_3 rt_geometry::get_position(float u, float v) restrict(amp) restrict(cpu)
{
	//stub since virtual methods are not allowed
	float_3 blank;
	return blank;
}

int rt_geometry::get_type() restrict(amp) restrict(cpu)
{
	return this->m_type;
}

int rt_geometry::get_material_index() restrict(amp) restrict(cpu)
{
	return this->m_material_index;
}