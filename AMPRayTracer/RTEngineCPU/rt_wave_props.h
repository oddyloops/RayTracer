#pragma once

#include <vector>

using namespace std;

namespace rt
{
	class rt_wave_props
	{
	public: 
		static vector<float> reflect(vector<float> normal,vector<float> light_dir);
	};
}