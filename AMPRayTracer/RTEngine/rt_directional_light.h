#pragma once
#include "rt_geometry.h"

using namespace rt_support::geometries;

namespace rt_support
{
	namespace lights
	{

		class rt_directional_light
		{
		public:


			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray
			///</summary>
			static float percent_light(array_view<rt_geometry, 1>& geom, float_3 geomPoint, int exceptGeomIndex, float_3& direction) restrict(amp);
		};

	}
}
