#pragma once

#include <vector>

#include "matrix.h"
#include "pixel_data.h"
#include "rt_camera.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "rt_core.h"

using namespace std;

using namespace rt;
using namespace rt_support;
using namespace rt_support::geometries;

typedef vector<vector<vector<float>>> scene_results;

class rt_gateway
{
public:
	

	static scene_results ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles,  rt_camera camera,image_spec spec);

};

