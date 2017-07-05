#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include "indexed_resource.h"
using namespace concurrency;
using namespace concurrency::graphics;

namespace rt_support
{
	namespace scene_resource_support
	{
		//helper class for instantiating array view
		class scene_resource_gateway
		{
			scene_resource_gateway(int size);

			array_view<indexed_resource> get_resource
		};
		/// <summary>
		/// Template class for storing resources in the scene database.
		/// Notice, this is an un-ordered collection of resources with index.
		/// e.g., when we want to find Light, index 2, we DO NOT look for it under
		///     mCollection[2]
		/// instead, we loop through the mCollection and look at the index of each resource.
		/// </summary>

		class scene_resource
		{
		private:
			array<indexed_resource, 1> m_collection;
			int m_count;
			int m_curr_index = 0;
		public:
			/// <summary>
			/// Constructor
			/// </summary>
			scene_resource(int count);

			/// <summary>
			/// Insert
			/// </summary>
			/// <param name="r"></param>
			void add_resource(indexed_resource r);

			/// <summary>
			/// Number of resources in the collection
			/// </summary>
			int get_current_count() restrict(amp, cpu);

			/// <summary>
			/// Find/return a resource with index. Null if none found
			/// </summary>
			/// <param name="index">Index of the resource to be found</param>
			/// <returns></returns>
			indexed_resource resource_lookup(int index) restrict(amp, cpu);


		};
	}
}