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
			vector<float> m_vertices[3];
			vector<float> m_normal;
			vector<float> m_u_vec, m_v_vec;
			float md;

			void initialize_triangle();

			/// <summary>
			/// Returns if the given pt is inside(true) or outside(false) of the polygon.
			/// </summary>
			/// <param name="pt"></param>
			/// <returns></returns>
			bool inside_polygon(vector<float> pt);

		public:
			__declspec(dllexport) rt_triangle();


			/// <summary>
			/// Construting a triangle from given vertices.
			/// </summary>
			/// <param name="v"></param>
			__declspec(dllexport) rt_triangle(vector<float> vertices[], matrix xform);


			/// <summary>
			/// Construting a triangle from given vertices.
			/// </summary>
			/// <param name="v"></param>
			__declspec(dllexport) rt_triangle(vector<float> vertices[]);

			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			bool intersect(ray& ray, intersection_record& record);

			/// <summary>
			/// pt is a position on in the rectangle, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position inside the rectangle (no error checking!)</param>
			/// <param name="bc">barrycentric coordinate of hit point (for triangle only)</param>
			/// <param name="u">returned normalized u value</param>
			/// <param name="v">returned normalized v value</param>
			void get_uv(vector<float> pt, vector<float> bc, float& u, float& v);

			/// <summary>
			/// recreives (u,v) and returns the object position that corresponds to the (u,v) coordinate.
			/// </summary>
			/// <param name="u">normalized u</param>
			/// <param name="v">normalized v</param>
			/// <returns>A position that cooresponds to (u,v) on the geometry </returns>
			vector<float> get_position(float u, float v);

			/// <summary>
			/// Returns the "approximated center" of the triangle. By simple averaging.
			/// </summary>
			/// <returns></returns>
			vector<float> get_center();

			
			vector<float> get_normal();

			vector<float> get_max();
			vector<float> get_min();

			vector<float> get_vertex(int i);

		};
	}
}