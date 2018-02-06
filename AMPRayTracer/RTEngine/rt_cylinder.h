#pragma once
#include "intersection_record.h"
#include "ray.h"

using namespace rt_support::ray_support;

namespace rt_support
{
	namespace geometries
	{
		class rt_cylinder
		{
		public:

			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			static int intersect(ray& ray, intersection_record& record, float& hit_distance, float_3& hitPt, float_3& normal, int& dist_index, float& height, float_3& axis_dir, float_3& base_pt, float_3& top_pt, float& radius_sq, float& orth_d)  restrict(amp);

			

			/// <summary>
			/// pt is a position on in the rectangle, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position inside the rectangle (no error checking!)</param>
			/// <param name="bc">barrycentric coordinate of hit point (for triangle only)</param>
			/// <param name="n">normal at intersection point</param>
			/// <param name="dist_index">determines if the intersection is on the circle ends or the side curves </param>
			/// <param name="u">returned normalized u value</param>
			/// <param name="v">returned normalized v value</param>
			static void get_uv(float_3& pt, float_3& bc,  float& u, float& v, int& dist_index, float_3& base_pt, float_3& top_pt,float_3& axis_dir, float_3& hor_axis_1, float_3& hor_axis_2,
				float& curve_section, float& flat_section, float& height, float& radius) restrict(amp);

			
		};
	}
}