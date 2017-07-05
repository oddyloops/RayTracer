#pragma once

#include <vector>


using namespace std;


namespace rt_support
{
	namespace ray_support
	{
		/// <summary>
		/// This is the RAY in ray tracing.
		/// </summary>

		class ray
		{
		private:
			vector<float> m_origin;   // origin of the ray
			vector<float> m_direction;   // direction of the ray, normalized


		public:
			ray();

			/// <summary>
			/// constructor based on two points.
			/// </summary>
			/// <param name="o">Origin of ray.</param>
			/// <param name="at">Any point in the path of the ray.</param>
			ray(vector<float> o, vector<float> at);

			/// <summary>
			/// Accessing functions.
			/// </summary>
			vector<float> get_origin();

			vector<float> get_direction();

			/// <summary>
			/// Creates a ray from orgin and a direction.
			/// </summary>
			/// <param name="org">Origin of the ray.</param>
			/// <param name="dir">Direction of the ray (assumed to be normalized!!)</param>
			/// <returns>A Ray</returns>
			static ray create_ray_from_pt_dir(vector<float> org, vector<float> dir);


		};
	}
}

