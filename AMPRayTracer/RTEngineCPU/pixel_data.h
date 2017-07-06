#pragma once
#include <vector>

using namespace std;

namespace rt_support
{
	// helper class that holds data fro current pixel
	class pixel_data
	{
	private:
		vector<float> m_default_color = { 0.8f,0.5f,0.5f };
		vector<float> m_pixel_position; // pixel position
		vector<float> m_pixel_color; // pixel color 
		float m_pixel_coverage; // pixel mask value
		float m_pixel_depth; //pixel depth value
		float m_hit_distance; //hit distance

		void set_pixel_data(vector<float> position, vector<float> color, float coverage, float depth, float hitDistance);

	public:
		pixel_data();

		pixel_data(vector<float> position,float hit_distance);

		pixel_data(vector<float> position, vector<float> color, float coverage, float depth, float hitDistance);

		vector<float> get_default_color();

		vector<float> get_pixel_position();

		vector<float> get_pixel_color();

		float get_pixel_depth();

		float get_pixel_coverage();

		float get_hit_distance();
	};
}