#pragma once
#include "rt_geometry.h"


namespace rt_support
{
	namespace geometries
	{
		/// <summary>
		/// A Rectangle
		/// </summary>

		class rt_rectangle : public rt_geometry
		{
		private:
			float_3 m_vertices[4];    // always 4 vertices
			int m_u_axis_index, m_v_axis_index; // 0 is X, 1 is Y, and 2 is Z
			float ma, mb, mc;
			float md;   // AX + BY + CZ + D = 0, this is the D
			float_3 m_true_normal; // normal to the polygon

									 // for texture mapping
			float_3 m_u_vec, m_v_vec; // Normalized vector in the U and V direction
			float m_u_size, m_v_size; // entire object size in the U and V direction
									
			/// <summary>
			/// Initialize internal data structures for intersection and texture support
			/// </summary>
			void initialize_rectangle() restrict(amp,cpu);

			/// <summary>
			/// Returns if the given pt is inside(true) or outside(false) of the polygon.
			/// </summary>
			/// <param name="pt"></param>
			/// <returns></returns>
			int inside_polygon(float_3 pt) restrict(amp);

			



									
		public:

			__declspec(dllexport) rt_rectangle()  restrict(amp, cpu);
			/// <summary>
			/// Constructs from parameters and then intialize for intersection computation.
			/// </summary>

			__declspec(dllexport) rt_rectangle(float_3 vertices[], concurrency::array<float, 2> xform);

			/// <summary>
			/// Construting a rectange from given vertices.
			/// </summary>
			/// <param name="v"></param>
			__declspec(dllexport) rt_rectangle(float_3 v[]) restrict(amp,cpu);

			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			int intersect(ray& r, intersection_record& record, array_view<float_3, 3>* bitmaps, array_view<float_3, 1>* scalars
				, array_view<float, 3>* f_bitmaps, array_view<float, 1>* f_scalars) restrict(amp);

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
			/// Returns the "approximated center" of the rectangle. By simple averaging.
			/// </summary>
			/// <returns></returns>
			float_3 get_center() restrict(amp);

			/// <summary>
			/// Accessing functions:
			/// </summary>
			float get_u_size() restrict(amp);
			float get_v_size() restrict(amp);

			float_3 get_max() restrict(amp);
			float_3 get_min() restrict(amp);

			float_3 get_vertex(int i) restrict(amp,cpu);




		};
	}
}

