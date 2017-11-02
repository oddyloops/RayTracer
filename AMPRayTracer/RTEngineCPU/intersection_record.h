#pragma once

#include <vector>

#include "ray.h"

using namespace std;

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
			vector<float> m_point;     // Interseciton position
			float m_u, m_v; //U-V at intersection
			vector<float> m_bc;        // Barry centric coordinate for triangle
			vector<float> m_normal;    // Normal at the intersection position
			vector<float> m_ray_dir;    // ray direciton that caused the intesection
			float m_hit_distance; // distance from the mPoint to the origin of the interesecting ray
			int m_geom_index;     // index into the geometry (of the global geometry) array
			int m_material_index; // index into the material array (of the global material) array
			int m_type; //type of geometry intersected

		public:
			/// <summary>
			/// Default Constructor.
			/// </summary>
			intersection_record();

			/// <summary>
			/// Contructs with a default hitDistance
			/// </summary>
			/// <param name="hitDist"></param>
			intersection_record(float hit_dist);

			/// <summary>
			/// Initialize the record to nothing is visible (nothing has intersected)
			/// </summary>
			void initialize_record();

			/// <summary>
			/// Sets the record with new data from an valid intersection.
			/// </summary>
			/// <param name="dist">distance to the intersection</param>
			/// <param name="intersectionPt">the visible (intersection) point</param>
			/// <param name="normal">normal vector at the visible point</param>
			/// <param name="ray">ray that caused the intersection</param>
			/// <param name="matIndex">material index at the intersection point</param>
			/// <param name="geomIndex">geometry index at the interesection point</param>
			void update_record(float dist, vector<float> intersection_pt, vector<float> normal, ray ray, int mat_index, int geom_index, int type,float u, float v);

			void update_bc(float u, float v, float w);


			/// <summary>
			/// Accessing functions
			/// </summary>
			float get_hit_distance();

			int get_material_index();

			void set_material_index(int index);

			int get_geom_index();

			void set_geom_index(int index);

			vector<float> get_intersection_position();

			vector<float> get_normal_at_intersect();

			vector<float> get_ray_direction();

			vector<float> get_hit_pt_bc();

			void set_normal_at_intersection(vector<float> n);

			int get_type();

			void set_type(int type);

			void set_uv(float u, float v);

			float get_u() const;

			float get_v() const;




		};
	}
}


