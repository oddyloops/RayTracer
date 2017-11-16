#pragma once
#include "rt_geometry.h"

  


namespace rt_support
{
	namespace geometries
	{
		class rt_sphere : public rt_geometry
		{
		private:
			vector<float> m_center;
			float m_radius;
			float m_radius_squared;

			//to allow non-axis aligned spheres
			vector<float> m_vertical_axis;
			vector<float> m_hor_axis_1;
			vector<float> m_hor_axis_2; //perp to axis 1 
			vector<float> m_base_point; //bottom of the sphere
		
		public:
			__declspec(dllexport) rt_sphere(vector<float> center, float radius);

			__declspec(dllexport) rt_sphere(vector<float> center, float radius, vector<float> vertical_axis);

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
			/// Returns the "approximated center" of the rectangle. By simple averaging.
			/// </summary>
			/// <returns></returns>
			vector<float> get_center();
			float get_radius();

			vector<float> get_max();
			vector<float> get_min();

			

		};
	}
}
