#pragma once
#define PI 3.14159265f
#include "vector_amp.h"
#include "ray.h"
#include <amp_math.h>

using namespace concurrency::fast_math;
using namespace rt_support::ray_support;
///<summary>
///Provides helper math functions
///</summary>

class math_util
{

public:
	static float deg_to_rad(float deg) restrict(amp, cpu);

	static float rad_to_degree(float rad) restrict(amp, cpu);

	static float abs(float x) restrict(amp, cpu);

	///<summary>
	///Computes the angle between two vectors (v1,v2) in the clock-wise direction
	///The normal is used to compute the determinant of the v1, v2, normal matrix
	///</summary>
	static float clock_wise_angle(float_3 v1, float_3 v2, float_3 normal) restrict(amp);

	//computes shortest distance between a point and a plane

	static float plane_point_dist(float_3 pt, float_3 norm, float_3 plane_point) restrict(amp);


	/// <summary>
			/// Dot(norm, P) + d = 0 is the plane equation
			/// Intersects the ray with the plane returns T/F
			/// if T 
			///     dist is the distance between the ray and the plane (may be negative)
			/// if F
			///     dist is undefined. Ray and Plane is parallel
			///  
			/// **WARNING**: this function flips the norm vector if the plane is facing
			///              away from the ray directoin!!
			/// </summary>
			/// <param name="r">Ray to intersect the plane</param>
			/// <param name="norm">Normal (normalized) vector of the plane</param>
			/// <param name="d">Dot(norm, P) + d = 0.</param>
			/// <param name="dist">Returned: Distance between the ray to the plane if the ray is not paralle with the plane</param>
			/// <returns>True, if there is an interesection, False, if ray is parallel to the plane</returns>
	static int ray_plane_intersection(ray& r, float_3& norm, float d, float& dist, float_3 plane_point) restrict(amp);


	//determine if a point is within a circle
	static int inside_circle(float_3 point, float_3 center, float radius_sq) restrict(amp);

	static int inside_polygon(float_3& pt, float_3 vertices[], float_3& u_vec, float_3& true_normal) restrict(amp);


	static int inside_triangle(float_3& pt, float_3 vertices[], float_3& u_vec,float_3& v_vec, float_3& true_normal) restrict(amp);

};