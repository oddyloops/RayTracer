#pragma once




#include "ray.h"
#include "intersection_record.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "pixel_data.h"
#include "scene_database.h"


using namespace std;


using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
using namespace rt_support::scene_resource_support;


namespace rt
{
	class rt_shader
	{
		scene_database* m_db;
	public:
		rt_shader() restrict(amp,cpu);
		rt_shader(scene_database* db) restrict(amp,cpu);

		float_3 compute_shade(intersection_record rec, int generation) restrict(amp);
	};
}