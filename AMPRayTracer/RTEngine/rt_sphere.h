#pragma once
#include "rt_geometry.h"


namespace rt_support
{
	namespace geometries
	{

		//contain all sphere computations
		class rt_sphere
		{
		
		
		public:
			

			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			static int intersect(ray& r, intersection_record& record, float& dist, float_3& pt, float_3& normal, float_3& center, float& radius)  restrict(amp);

			
			
			/// <summary>
			/// pt is a position on in the rectangle, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position inside the sphere (no error checking!)</param>
			/// <param name="bc">barrycentric coordinate of hit point (for triangle only)</param>
			/// <param name="u">returned normalized u value</param>
			/// <param name="v">returned normalized v value</param>
			static void get_uv(float_3& pt, float_3& bc, float& u, float& v, float& radius,float_3& base_point, float_3& axis_dir, float_3& hor_axis, float_3& hor_axis_perp) restrict(amp);

			
		};
	}
}
