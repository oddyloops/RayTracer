#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include <vector>
#include "rt_camera.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "rt_core.h"


using namespace concurrency;
using namespace concurrency::graphics;
using namespace std;

using namespace rt;
using namespace rt_support;
using namespace rt_support::geometries;

struct scene_results
{
	vector<float_3> color;
	vector<float> coverage;
	vector<float> depth;
};

class rt_gateway
{
public:
	

	static scene_results ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles,  rt_camera camera,image_spec spec);

};

