#pragma once


#include "ray.h"
#include "intersection_record.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "rt_triangle.h"
#include "rt_cylinder.h"
#include "rt_plane.h"

using namespace std;

using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
using namespace rt_support::scene_resource_support;

namespace rt
{
	///<summary>
	///Handles all ray-geometry intersection computations
	///</summary>
	class rt_visibility
	{
		int dummy; //used to force object to be a multiple of 32 -bit (Requirement in the GPU kernel code)
	public:
		rt_visibility() restrict(amp, cpu);

		void compute_visibility(ray r, int except_geom_index, intersection_record& rec, array_view<rt_sphere,1>* spheres, array_view<rt_rectangle,1>* rects,
			array_view<rt_triangle, 1>* m_triangles, array_view<rt_plane, 1>* m_planes, array_view<rt_cylinder, 1>* m_cylinders) restrict(amp);
	};
}