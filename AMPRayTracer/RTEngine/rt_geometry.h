#pragma once
#include<amp.h>
#include <amp_graphics.h>
#include "ray.h"
#include "intersection_record.h"
#include "indexed_resource.h"
#include "texture_map.h"


using namespace concurrency;
using namespace concurrency::graphics;
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
			static const int plane = 4;
			static const int cylinder = 5;
		
		};

		///<summary>
		///Parent class containing basic specifications for a geometry
		///</summary>
		class rt_geometry  : public indexed_resource
		{
		private:
			float_3 m_min;
			float_3 m_max;
			
		protected:
			int m_type;
			int m_material_index;
			texture_map<float_3> m_normal_map;
			texture_map<float> m_bump_map;


			/// <summary>
			/// Dot(norm, P) + d = 0 is the plane equation
			/// Intersects the ray with the plane returns T/F
			/// if T 
			///     dist is the distance between the ray and the plane (may be negative)
			/// if F
			///     dist is undefined. Ray and Plane is parallel
			///  
			/// **WARNING**: this function flips the norm vector if the plane is facing
			///              away from the ray directoin!!
			/// </summary>
			/// <param name="r">Ray to intersect the plane</param>
			/// <param name="norm">Normal (normalized) vector of the plane</param>
			/// <param name="d">Dot(norm, P) + d = 0.</param>
			/// <param name="dist">Returned: Distance between the ray to the plane if the ray is not paralle with the plane</param>
			/// <returns>True, if there is an interesection, False, if ray is parallel to the plane</returns>
			int ray_plane_intersection(ray& r, float_3& norm, float d, float& dist,float_3 plane_point) restrict(amp);

			float plane_point_dist(float_3 pt, float_3 norm, float_3 plane_point) restrict(amp);
		public:
			

			__declspec(dllexport) void set_material_index(int index);


			/// <summary>
			/// Parse command line for <xform> </xform> that is embedeed inside 
			/// Geometry.
			/// </summary>
			static concurrency::array<float,2> parse_xform(float_3 translation, float rx, float ry, float rz, float_3 scale) restrict(cpu);

			/// <summary>
			/// Returns status of if Ray intersects with this Geom. If so, details of intersection
			/// is returned in the IntersectionRecord.
			/// </summary>
			/// <param name="r">Incoming ray.</param>
			/// <param name="record">If intersect, this record has the details.</param>
			/// <returns>T/F: intersect or not.</returns>
			int intersect(ray& r, intersection_record& record) restrict(amp);

			/// <summary>
			/// Returns status of if Ray intersects with this Geom. If so, details of intersection
			/// is returned in the IntersectionRecord.
			/// </summary>
			/// <param name="r">Incoming ray.</param>
			/// <param name="record">If intersect, this record has the details.</param>
			/// <returns>T/F: intersect or not.</returns>
			int intersect(ray& r, intersection_record& record, array_view<float_3, 3>* bitmaps, array_view<float_3, 1>* scalars
				, array_view<float, 3>* f_bitmaps, array_view<float, 1>* f_scalars) restrict(amp);

			/// <summary>
			/// Min point
			/// </summary>
			/// <returns></returns>
			float_3 get_min() restrict(amp,cpu);

			/// <summary>
			/// Max point
			/// </summary>
			/// <returns></returns>
			float_3 get_max() restrict(amp,cpu);

			/// <summary>
			/// pt is a position on the geometry, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position on the geometry (no error checking!)</param>
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
			float_3 get_position(float u, float v) restrict(amp,cpu);

			int get_type() restrict(amp,cpu);

			int get_material_index() restrict(amp,cpu);

			__declspec(dllexport) void set_normal_map(texture_map<float_3> normal_map) restrict(amp, cpu);

			__declspec(dllexport) void set_bump_map(texture_map<float> bump_map) restrict(amp, cpu);

			float_3 get_normal(float u, float v,array_view<float_3,3>* bitmaps,array_view<float_3,1>* scalars) restrict(amp);

		};
	}
}
