#pragma once
#include <vector>
#include "rt_sphere.h"
#include "rt_rectangle.h"

using namespace std;
using namespace rt_support::geometries;

namespace rt_support
{
	namespace scene_resource_support
	{
		template<class T>
		class scene_resource
		{
		private:
			vector<rt> m_collection;
		public:
			scene_resource();
			
			void add_resource(indexed_resource r);

		};
	}
}