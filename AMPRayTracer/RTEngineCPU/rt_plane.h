#pragma once
#include "rt_geometry.h"

namespace rt_support
{
	namespace geometries
	{
		/// <summary>
		/// An infinite Plane
		/// </summary>

		class rt_plane : public rt_geometry
		{
		private:
			
			vector<float> point_0;
			vector<float> m_true_normal;
			float md, m_map_width, m_map_height;
			vector<float> m_u_vec, m_v_vec;

			

	
		public:
			__declspec(dllexport) rt_plane();


			/// <summary>
			/// Construting a plane from three non co-linear points (had to explicity specify a map dimension because a plane is infinite
			/// </summary>
			/// <param name="v"></param>
			__declspec(dllexport) rt_plane(vector<float> points[], float map_width, float map_height);


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


		};
	}
}