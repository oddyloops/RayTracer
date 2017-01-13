#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include <vector>
#include "pixel_data.h"
#include "rt_camera.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "rt_core.h"
#include "rt_material.h"

using namespace concurrency;
using namespace concurrency::graphics;
using namespace std;

using namespace rt;
using namespace rt_support;
using namespace rt_support::geometries;


class rt_gateway
{
public:
	static vector<float_3> m_image;
	static vector<float_3> m_coverage_mask;
	static vector<float_3> m_depth_map;

	static void ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles, vector<rt_material> materials, rt_camera camera,int samples_per_pixel);

};

