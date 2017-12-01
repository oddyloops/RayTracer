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

	//initializes the required parameters and initiates the ray tracing process

	__declspec(dllexport) static scene_results ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles,
		vector<rt_triangle> triangles, vector<rt_plane> planes, vector<rt_cylinder> cylinders,
		vector<rt_material> materials, vector<rt_directional_light> d_lights,
		vector<rt_point_light> p_lights, vector<rt_spot_light> s_lights,
		vector<rt_area_light> a_lights, float_3 ambience_color, float ambience_intensity,
		rt_camera camera, image_spec spec, map<int, tuple<string, int, int>> _vec_bmps , vector<float_3> scalars,
		map<int, tuple<string, int, int>> _flt_bmps , vector<float> f_scalars, int max_bmp_width = 1920, int max_bmp_height = 1080);

	
	static void load_vec_bmps(vector<float_3>& buffer, map<int, tuple<string, int, int>>& bmps, int max_width, int max_height);

	static void load_flt_bmps(vector<float>& buffer, map<int, tuple<string, int, int>>& bmps, int max_width, int max_height);
};


