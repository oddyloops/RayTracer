#pragma once

#include <vector>

#include "ray.h"
#include "intersection_record.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "scene_database.h"

using namespace std;

using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
using namespace rt_support::scene_resource_support;

namespace rt
{

	class rt_visibility
	{
	private: 
		scene_database* m_db;
	public:
		rt_visibility();
		rt_visibility(scene_database* db);

		void compute_visibility(ray r, int except_geom_index, intersection_record& rec);
	};
}