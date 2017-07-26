#pragma once

#include <vector>

#include "ray.h"
#include "intersection_record.h"
#include "indexed_resource.h"
#include "matrix.h"

using namespace std;

using namespace rt_support::ray_support;
using namespace rt_support::scene_resource_support;

namespace rt_support
{
	namespace geometries
	{
		struct rt_geometry_type
		{
			static const int sphere = 1;
			static const int rectangle = 2;
			static const int triangle = 3;
		};

		class rt_geometry  : public indexed_resource
		{
		private:
			vector<float> m_min;
			vector<float> m_max;
			
		protected:
			int m_type;
			int m_material_index;


		
			bool ray_plane_intersection(ray& r, vector<float> norm, float a,float b, float c, float d, float* dist);
		public:
			

			/// <summary>
			/// Parse command line for <xform> </xform> that is embedeed inside 
			/// Geometry.
			/// </summary>
			static matrix parse_xform(vector<float> translation, float rx, float ry, float rz, vector<float> scale);

			/// <summary>
			/// Returns status of if Ray intersects with this Geom. If so, details of intersection
			/// is returned in the IntersectionRecord.
			/// </summary>
			/// <param name="r">Incoming ray.</param>
			/// <param name="record">If intersect, this record has the details.</param>
			/// <returns>T/F: intersect or not.</returns>
			bool intersect(ray& r, intersection_record& record);

			/// <summary>
			/// Min point
			/// </summary>
			/// <returns></returns>
			vector<float> get_min();

			/// <summary>
			/// Max point
			/// </summary>
			/// <returns></returns>
			vector<float> get_max();

			/// <summary>
			/// pt is a position on the geometry, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position on the geometry (no error checking!)</param>
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

			int get_type();

			int get_material_index();


		};
	}
}
