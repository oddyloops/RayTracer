#pragma once
#include "rt_geometry.h"

namespace rt_support
{
	namespace geometries
	{
		class rt_cylinder : public rt_geometry
		{
		private:
			float m_radius;
			float m_radius_sq;
			float m_height;
			float_3 m_top_center;
			float_3 m_base_center;
			float_3 m_axis_dir;
			float_3 m_hor_axis_dir;
			float_3 m_hor_axis_dir_perp;
			float m_orth_d; //d for orthogonal plane
			float m_curve_section; //the section of v occupied by curved surface for u-v mapping
			float m_flat_section; //the section of v occupied by a single flat circular surface
			int m_dist_index; //used to determine which part of the cylinder was hit

			int inside_circle(float_3 point, int is_top_circle) restrict(amp);
		public:
			__declspec(dllexport) rt_cylinder() restrict(amp,cpu);

			__declspec(dllexport) rt_cylinder(float radius, float_3 top_center, float_3 base_center) restrict(amp,cpu);


			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			int intersect(ray& ray, intersection_record& record)  restrict(amp);

			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			int intersect(ray& ray, intersection_record& record, texture<float_3, 3>* bitmaps, texture<float_3, 1>* scalars
				, texture<float, 3>* f_bitmaps, texture<float, 1>* f_scalars) restrict(amp);

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
			void get_uv(float_3 pt, float_3 bc, float_3 n, int dist_index, float& u, float& v) restrict(amp);

			/// <summary>
			/// recreives (u,v) and returns the object position that corresponds to the (u,v) coordinate.
			/// </summary>
			/// <param name="u">normalized u</param>
			/// <param name="v">normalized v</param>
			/// <returns>A position that cooresponds to (u,v) on the geometry </returns>
			float_3 get_position(float u, float v) restrict(amp);

			/// <summary>
			/// Returns the "approximated center" of the cylinder
			/// </summary>
			/// <returns></returns>
			float_3 get_center() restrict(amp);
			float get_radius() restrict(amp);

			float_3 get_max() restrict(amp);
			float_3 get_min() restrict(amp);

		};
	}
}