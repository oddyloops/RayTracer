#pragma once
#include "rt_geometry.h"

namespace rt_support
{
	namespace geometries
	{
		/// <summary>
		/// A Triangle
		/// </summary>

		class rt_triangle : public rt_geometry
		{
		private:
			float_3 m_vertices[3];
			float_3 m_true_normal;
			float_3 m_u_vec, m_v_vec;
			float md;
			float m_apex_u;

			void initialize_triangle() restrict(amp,cpu);

			/// <summary>
			/// Returns if the given pt is inside(true) or outside(false) of the polygon.
			/// </summary>
			/// <param name="pt"></param>
			/// <returns></returns>
			int inside_polygon(float_3 pt) restrict(amp);

			//computes bary-centric coordinates
			float_3 get_bc(float_3 pt)  restrict(amp);

		public:
			__declspec(dllexport) rt_triangle() restrict(amp,cpu);


			/// <summary>
			/// Construting a triangle from given vertices.
			/// </summary>
			/// <param name="v"></param>
			__declspec(dllexport) rt_triangle(float_3 vertices[], concurrency::array<float, 2> xform);


			/// <summary>
			/// Construting a triangle from given vertices.
			/// </summary>
			/// <param name="v"></param>
			__declspec(dllexport) rt_triangle(float_3 vertices[]) restrict(amp,cpu);

			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			int intersect(ray& r, intersection_record& record) restrict(amp);

			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			int intersect(ray& r, intersection_record& record, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
				, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp);

			/// <summary>
			/// pt is a position on in the rectangle, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position inside the rectangle (no error checking!)</param>
			/// <param name="bc">barrycentric coordinate of hit point (for triangle only)</param>
			/// <param name="u">returned normalized u value</param>
			/// <param name="v">returned normalized v value</param>
			void get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp);

			/// <summary>
			/// recreives (u,v) and returns the object position that corresponds to the (u,v) coordinate.
			/// </summary>
			/// <param name="u">normalized u</param>
			/// <param name="v">normalized v</param>
			/// <returns>A position that cooresponds to (u,v) on the geometry </returns>
			float_3 get_position(float u, float v) restrict(amp);

			/// <summary>
			/// Returns the "approximated center" of the triangle. By simple averaging.
			/// </summary>
			/// <returns></returns>
			float_3 get_center() restrict(amp);


			

			float_3 get_max() restrict(amp);
			float_3 get_min() restrict(amp);

			float_3 get_vertex(int i) restrict(amp);

		};
	}
}