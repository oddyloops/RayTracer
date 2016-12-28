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
			float md;   // AX + BY + CZ + D = 0, this is the D
			float_3 m_normal; // normal to the polygon

									 // for texture mapping
			float_3 mu_vec, mv_vec; // Normalized vector in the U and V direction
			float mu_size, mv_size; // entire object size in the U and V direction

									
		public:
			/// <summary>
			/// Constructs from parameters and then intialize for intersection computation.
			/// </summary>

			rt_rectangle(float_3 vertices, int material_index,concurrency::array<float> xform);




		};
	}
}

