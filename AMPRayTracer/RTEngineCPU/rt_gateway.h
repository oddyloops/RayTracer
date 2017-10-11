#pragma once

#include <vector>

#include "matrix.h"
#include "rt_camera.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "rt_triangle.h"
#include "rt_plane.h"
#include "rt_cylinder.h"
#include "rt_cube.h"
#include "rt_core.h"

using namespace std;

using namespace rt;
using namespace rt_support;
using namespace rt_support::scene_resource_support;
using namespace rt_support::lights;
using namespace rt_support::geometries;

typedef vector<vector<vector<float>>> scene_results;

class rt_gateway
{
public:
	

	static __declspec(dllexport) scene_results ray_trace(vector<rt_sphere> spheres, vector<rt_rectangle> rectangles,
		vector<rt_triangle> triangles,vector<rt_plane> planes,vector<rt_cylinder> cylinders, vector<rt_cube> cubes,
		vector<rt_material> materials,vector<rt_directional_light> d_lights,
		vector<rt_point_light> p_lights, vector<rt_spot_light> s_lights,
		vector<rt_area_light> a_lights,vector<float> ambience_color,float ambience_intensity,
		rt_camera camera,image_spec spec);

};

