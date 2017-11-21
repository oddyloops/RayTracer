#pragma once

#include <amp.h>
#include <amp_graphics.h>
#include "ray.h"
using namespace concurrency;
using namespace concurrency::graphics;


namespace rt_support
{
	namespace ray_support
	{
		/// <summary>
		/// Records important information at each visible intersection
		/// </summary>

		class intersection_record
		{
		private:
			float_3 m_point;     // Interseciton position
			float_3 m_bc;        // Barry centric coordinate for triangle
			float_3 m_normal;    // Normal at the intersection position
			float_3 m_ray_dir;    // ray direciton that caused the intesection
			float m_hit_distance; // distance from the mPoint to the origin of the interesecting ray
			int m_geom_index;     // index into the geometry (of the global geometry) array
			int m_material_index; // index into the material array (of the global material) array
			int m_type; //type of geometry intersected
			float m_u, m_v; //u-v coordinates

		public:
			/// <summary>
			/// Default Constructor.
			/// </summary>
			intersection_record() restrict(amp);

			/// <summary>
			/// Contructs with a default hitDistance
			/// </summary>
			/// <param name="hitDist"></param>
			intersection_record(float hit_dist) restrict(amp);

			/// <summary>
			/// Initialize the record to nothing is visible (nothing has intersected)
			/// </summary>
			void initialize_record() restrict(amp);

			/// <summary>
			/// Sets the record with new data from an valid intersection.
			/// </summary>
			/// <param name="dist">distance to the intersection</param>
			/// <param name="intersectionPt">the visible (intersection) point</param>
			/// <param name="normal">normal vector at the visible point</param>
			/// <param name="ray">ray that caused the intersection</param>
			/// <param name="matIndex">material index at the intersection point</param>
			/// <param name="geomIndex">geometry index at the interesection point</param>
			void update_record(float dist, float_3 intersection_pt, float_3 normal, ray ray, int mat_index, int geom_index,int type) restrict(amp);

			/// <summary>
			/// Sets the record with new data from an valid intersection.
			/// </summary>
			/// <param name="dist">distance to the intersection</param>
			/// <param name="intersectionPt">the visible (intersection) point</param>
			/// <param name="normal">normal vector at the visible point</param>
			/// <param name="ray">ray that caused the intersection</param>
			/// <param name="matIndex">material index at the intersection point</param>
			/// <param name="geomIndex">geometry index at the interesection point</param>
			void force_update_record(float dist, float_3 intersection_pt, float_3 normal, ray ray, int mat_index, int geom_index, int type) restrict(amp);

			void update_bc(float u, float v, float w) restrict(amp);


			/// <summary>
			/// Accessing functions
			/// </summary>
			float get_hit_distance() restrict(amp);

			int get_material_index() restrict(amp);

			int get_geom_index() restrict(amp);

			float_3 get_intersection_position() restrict(amp);

			float_3 get_normal_at_intersect() restrict(amp);

			float_3 get_ray_direction() restrict(amp);

			float_3 get_hit_pt_bc() restrict(amp);

			void set_normal_at_intersection(float_3 n) restrict(amp);

			int get_type() restrict(amp);

			void set_uv(float u, float v) restrict(amp);

			float get_u() restrict(amp);

			float get_v() restrict(amp);


		};
	}
}


