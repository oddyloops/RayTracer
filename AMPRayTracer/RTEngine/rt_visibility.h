#pragma once

#include <vector>

#include "ray.h"
#include "intersection_record.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"

using namespace std;

using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
using namespace rt_support::scene_resource_support;

namespace rt
{

	class rt_visibility
	{
	
	public:
		rt_visibility() restrict(amp, cpu);

		void compute_visibility(ray r, int except_geom_index, intersection_record& rec, array_view<rt_sphere,1>& spheres, array_view<rt_rectangle,1>& rects) restrict(amp);
	};
}