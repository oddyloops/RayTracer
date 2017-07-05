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

namespace rt
{

	class rt_visibility
	{
	public:
		rt_visibility();

		void compute_visibility(ray r, int except_geom_index, intersection_record& rec, vector<rt_sphere> spheres, vector<rt_rectangle> rectangles);
	};
}