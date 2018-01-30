#include "rt_triangle.h"
#include "vector_amp.h"
#include "matrix_amp.h"
#include "math_util.h"

using namespace rt_support::geometries;






int rt_triangle::intersect(ray& r, intersection_record& record,  float_3& pt, float& dist, float_3& normal,float_3& true_normal, float_3 vertices[], float_3& u_vec,
	float_3& v_vec, float& d) restrict(amp)
{

	float_3  n;

	n = true_normal;    // because ray/plane intersection may flip the normal!
	if (!math_util::ray_plane_intersection(r, n, d, dist, vertices[0]))
		return false;



	pt = r.get_origin() + (r.get_direction() * dist);

	/*
	* Now need to decide inside or outside
	*/
	
	if (!math_util::inside_triangle(pt,vertices,u_vec,v_vec,true_normal))
		return false;

	return true;
}



void rt_triangle::get_uv(float_3 pt, float_3 bc, float& u, float& v, float& apex_u) restrict(amp)
{
	//uv at v0 is 0,0  at v1 is 1,0 at v2 is m_apex_u,1
	u = bc.y + bc.z * apex_u;
	v = bc.z;
}

