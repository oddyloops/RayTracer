#pragma once
#include "all_map.h"
#include "matrix.h"

namespace rt_support
{
	namespace scene_resource_support
	{
		class vector_map : public all_map
		{
		private:
			vector<matrix<vector<float>>> m_bitmap_source; //for file sources
			vector<vector<float>> m_scalar_source; //for scalar sources


			vector<float> get_value_plain(float u, float v);

			vector<float> get_value_checkered(float u, float v);

			vector<float> get_value_stripes(float u, float v);

			vector<float> get_value_wavy(float u, float v);

		public:


			__declspec(dllexport) vector_map();

			__declspec(dllexport) vector_map(bool is_file_source, int map_type);

			__declspec(dllexport) void set_bitmap_source(vector<matrix<vector<float>>> bitmap_source);

			__declspec(dllexport) void set_scalar_source(vector<vector<float>> scalar_source);

			vector<float> get_value(float u, float v); //get value based on supplied u-v coordinates

			vector_map& operator=(const vector_map v);
		};
	}
}