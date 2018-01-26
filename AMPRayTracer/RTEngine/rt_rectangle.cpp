#include <math.h>
#include "rt_rectangle.h"
#include "math_util.h"
#include "matrix_amp.h"
#include "vector_amp.h"


using namespace rt_support::geometries;



int rt_rectangle::intersect(ray& r, intersection_record& record, float& dist, float_3& hitPt, float_3& normal, float_3& true_normal, float_3 vertices[4], float& d, float_3& u_vec) restrict(amp)
{
	

	normal = true_normal;    // because ray/plane intersection may flip the normal!
	if (!math_util::ray_plane_intersection(r, normal, d, dist, vertices[0]))
		return false;

	/*
	* rectangle behind the ray or there are other closer intersections
	*/
	if ((dist < 0) || (dist > record.get_hit_distance()))
		return false;

	hitPt = r.get_origin() + (r.get_direction() * dist);

	/*
	* Now need to decide inside or outside
	*/
	if (!math_util::inside_polygon(hitPt,vertices,u_vec,true_normal))
		return false;
	
	
	return true;

}



void rt_rectangle::get_uv(float_3 pt, float_3 bc, float& u, float& v, float_3& vertex0,
	float_3& u_vec, float_3& v_vec, float& u_size, float& v_size) restrict(amp)
{
	float_3 pt_norm = pt - vertex0;
	u = vector_amp::dot(pt_norm, u_vec) / u_size; //project vec on u dir
	v = vector_amp::dot(pt_norm, v_vec) / v_size; // project vec on v dir
}

