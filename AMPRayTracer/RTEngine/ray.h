#pragma once
#include <amp.h>
#include <amp_graphics.h>
using namespace concurrency;
using namespace concurrency::graphics;

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
			float_3 m_origin;   // origin of the ray
			float_3 m_direction;   // direction of the ray, normalized


		public:
			ray() restrict(amp);

			/// <summary>
			/// constructor based on two points.
			/// </summary>
			/// <param name="o">Origin of ray.</param>
			/// <param name="at">Any point in the path of the ray.</param>
			ray(float_3 o, float_3 at) restrict(amp);

			/// <summary>
			/// Accessing functions.
			/// </summary>
			float_3 get_origin() restrict(amp);

			float_3 get_direction() restrict(amp);

			/// <summary>
			/// Creates a ray from orgin and a direction.
			/// </summary>
			/// <param name="org">Origin of the ray.</param>
			/// <param name="dir">Direction of the ray (assumed to be normalized!!)</param>
			/// <returns>A Ray</returns>
			static ray create_ray_from_pt_dir(float_3 org, float_3 dir) restrict(amp);


		};
	}
}

