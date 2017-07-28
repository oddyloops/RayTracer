#pragma once

#include<vector>

using namespace std;

namespace rt
{
	class pixel_data
	{
	private:
		vector<float> m_color;
		float m_coverage;
		float m_depth;

	public:
		pixel_data();
		pixel_data& operator=(const pixel_data& cam);
		pixel_data(vector<float> color,float coverage, float depth);
		void set_color(vector<float> color);
		void set_coverage(float coverage);
		void set_depth(float depth);

		vector<float> get_color() const;
		float get_coverage() const;
		float get_depth() const;

		static vector<float> get_default_color();
	};
}
