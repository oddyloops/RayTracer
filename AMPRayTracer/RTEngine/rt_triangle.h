#pragma once
#include "intersection_record.h"
#include "ray.h"

using namespace rt_support::ray_support;

namespace rt_support
{
	namespace geometries
	{
		/// <summary>
		/// A Triangle
		/// </summary>

		class rt_triangle
		{

		public:
			
			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			static int intersect(ray& r, intersection_record& record, float_3& pt, float& dist, float_3& normal, float_3& true_normal, float_3 vertices[], float_3& u_vec,
				float_3& v_vec, float& d) restrict(amp);

			

			/// <summary>
			/// pt is a position on in the rectangle, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position inside the rectangle (no error checking!)</param>
			/// <param name="bc">barrycentric coordinate of hit point (for triangle only)</param>
			/// <param name="u">returned normalized u value</param>
			/// <param name="v">returned normalized v value</param>
			static void get_uv(float_3 pt, float_3 bc, float& u, float& v, float& apex_u) restrict(amp);

		
		};
	}
}