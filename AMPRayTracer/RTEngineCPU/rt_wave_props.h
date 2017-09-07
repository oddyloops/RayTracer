#pragma once

#include <vector>

using namespace std;

namespace rt
{
	class rt_wave_props
	{
	public: 
		static vector<float> reflect(vector<float> normal,vector<float> light_dir);

		static vector<float> refract(vector<float> normal, vector<float> light_dir, float src_refr_index, float dest_refr_index);
	};
}