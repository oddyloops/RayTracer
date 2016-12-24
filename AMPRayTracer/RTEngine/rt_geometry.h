#pragma once
#include<amp.h>
#include <amp_graphics.h>
#include "ray.h"
#include "intersection_record.h"
#include "indexed_resource.h"

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
			const int sphere = 1;
			const int rectangle = 2;
			const int triangle = 3;
		};

		class rt_geometry  : public indexed_resource
		{
		private:
			float_3 m_min;
			float_3 m_max;
			
		protected:
			int m_type;
			int m_material_index;

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

			bool ray_plane_intersection(ray& r, float_3 norm, float_3 d, float_3& dist) restrict(amp);
		public:
			/// <summary>
			/// Returns status of if Ray intersects with this Geom. If so, details of intersection
			/// is returned in the IntersectionRecord.
			/// </summary>
			/// <param name="r">Incoming ray.</param>
			/// <param name="record">If intersect, this record has the details.</param>
			/// <returns>T/F: intersect or not.</returns>
			bool intersect(ray& r, const intersection_record& record) restrict(amp) restrict(cpu);

			/// <summary>
			/// Min point
			/// </summary>
			/// <returns></returns>
			float_3 get_min() restrict(amp) restrict(cpu);

			/// <summary>
			/// Max point
			/// </summary>
			/// <returns></returns>
			float_3 get_max() restrict(amp) restrict(cpu);

			/// <summary>
			/// pt is a position on the geometry, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position on the geometry (no error checking!)</param>
			/// <param name="bc">barrycentric coordinate of hit point (for triangle only)</param>
			/// <param name="u">returned normalized u value</param>
			/// <param name="v">returned normalized v value</param>
			void get_uv(float_3 pt, float_3 bc, float& u, float& v) restrict(amp) restrict(cpu);

			/// <summary>
			/// recreives (u,v) and returns the object position that corresponds to the (u,v) coordinate.
			/// </summary>
			/// <param name="u">normalized u</param>
			/// <param name="v">normalized v</param>
			/// <returns>A position that cooresponds to (u,v) on the geometry </returns>
			float_3 get_position(float u, float v) restrict(amp) restrict(cpu);

			int get_type() restrict(amp) restrict(cpu);

			int get_material_index() restrict(amp) restrict(cpu);

		};
	}
}
