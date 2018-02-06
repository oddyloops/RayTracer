#pragma once


#include "ray.h"
#include "intersection_record.h"
#include "rt_geometry.h"
using namespace std;

using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
using namespace rt_support::scene_resource_support;

namespace rt
{
	///<summary>
	///Handles all ray-geometry intersection computations
	///</summary>
	class rt_visibility
	{
		int dummy; //used to force object to be a multiple of 32 -bit (Requirement in the GPU kernel code)
	public:
		rt_visibility() restrict(amp, cpu);

		void compute_visibility(ray r, int except_geom_index, intersection_record& rec, array_view<rt_geometry,1>* geom,  texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp);
	};
}