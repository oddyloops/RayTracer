#pragma once
#include "all_map.h"
#include "matrix.h"

namespace rt_support
{
	namespace scene_resource_support
	{
		class float_map : public all_map
		{
		private:
			vector<matrix<float>> m_bitmap_source; //for file sources
			vector<float> m_scalar_source; //for scalar sources

			float get_value_plain(float u, float v);

			float get_value_checkered(float u, float v); 

			float get_value_stripes(float u, float v);

			float get_value_wavy(float u, float v);

			

			
		public: 
			__declspec(dllexport) float_map();

			__declspec(dllexport) float_map(bool is_file_source, int map_type);

			__declspec(dllexport) void set_bitmap_source(vector<matrix<float>> bitmap_source);

			__declspec(dllexport) void set_scalar_source(vector<float> scalar_source);

			float get_value(float u, float v); //get value based on supplied u-v coordinates

			float_map& operator=(const float_map& f);

			
		};
	}
}
