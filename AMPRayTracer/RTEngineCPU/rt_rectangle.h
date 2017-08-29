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
			vector<float> m_vertices[4];    // always 4 vertices
			int m_u_axis_index, m_v_axis_index; // 0 is X, 1 is Y, and 2 is Z
			float ma, mb, mc;
			float md;   // AX + BY + CZ + D = 0, this is the D
			vector<float> m_normal; // normal to the polygon
			

									 // for texture mapping
			vector<float> m_u_vec, m_v_vec; // Normalized vector in the U and V direction
			float m_u_size, m_v_size; // entire object size in the U and V direction
									
			/// <summary>
			/// Initialize internal data structures for intersection and texture support
			/// </summary>
			void initialize_rectangle();

			
			/// <summary>
			/// Returns if the given pt is inside(true) or outside(false) of the polygon.
			/// </summary>
			/// <param name="pt"></param>
			/// <returns></returns>
			bool inside_polygon(vector<float> pt);
			
			float clock_wise_angle(vector<float> v1, vector<float> v2, vector<float> normal);


									
		public:


			rt_rectangle();

			rt_rectangle& operator=(const rt_rectangle& r);
			/// <summary>
			/// Constructs from parameters and then intialize for intersection computation.
			/// </summary>

			rt_rectangle(vector<float> vertices[],matrix xform);

			/// <summary>
			/// Construting a rectange from given vertices.
			/// </summary>
			/// <param name="v"></param>
			rt_rectangle(vector<float> v[]);

			

			/// <summary>
			/// Intersects the ray, if intersection is closer than the one inside the record,
			/// sets the records with intersection information.
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="record"></param>
			/// <returns></returns>
			bool intersect(ray& ray,intersection_record& record);

			/// <summary>
			/// pt is a position on in the rectangle, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position inside the rectangle (no error checking!)</param>
			/// <param name="bc">barrycentric coordinate of hit point (for triangle only)</param>
			/// <param name="u">returned normalized u value</param>
			/// <param name="v">returned normalized v value</param>
			void get_uv(vector<float> pt, vector<float> bc, float& u, float& v) ;

			/// <summary>
			/// recreives (u,v) and returns the object position that corresponds to the (u,v) coordinate.
			/// </summary>
			/// <param name="u">normalized u</param>
			/// <param name="v">normalized v</param>
			/// <returns>A position that cooresponds to (u,v) on the geometry </returns>
			vector<float> get_position(float u, float v);

			/// <summary>
			/// Returns the "approximated center" of the rectangle. By simple averaging.
			/// </summary>
			/// <returns></returns>
			vector<float> get_center();

			/// <summary>
			/// Accessing functions:
			/// </summary>
			float get_u_size();
			float get_v_size();
			vector<float> get_normal();

			vector<float> get_max();
			vector<float> get_min();

			vector<float> get_vertex(int i);




		};
	}
}

