#pragma once

#include<vector>



#include "ray.h"
#include "intersection_record.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "pixel_data.h"


using namespace std;


using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;

namespace rt
{
	class rt_shader
	{

	public:
		rt_shader();

		vector<float> compute_shade(intersection_record rec, int generation);
	};
}