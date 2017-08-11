#pragma once
#include <amp.h>
#include <amp_graphics.h>

using namespace concurrency;
using namespace concurrency::graphics;

namespace rt_support
{
	// helper class that holds data fro current pixel
	class pixel_data
	{
	private:
		float_3 m_default_color = float_3(1.0f,0.7f,0.7f);
		float_3 m_pixel_position; // pixel position
		float_3 m_pixel_color; // pixel color 
		float m_pixel_coverage; // pixel mask value
		float m_pixel_depth; //pixel depth value
		float m_hit_distance; //hit distance

		void set_pixel_data(float_3 position, float_3 color, float coverage, float depth, float hitDistance) restrict(amp,cpu);

	public:
		pixel_data() restrict(amp,cpu);

		pixel_data(float_3 color, float coverage, float depth) restrict(amp,cpu);

		pixel_data(float_3 position, float_3 color, float coverage, float depth, float hitDistance) restrict(amp,cpu);

		float_3 get_default_color() restrict(amp,cpu);

		float_3 get_pixel_position() restrict(amp, cpu);

		float_3 get_pixel_color() restrict(amp, cpu);

		float get_pixel_depth() restrict(amp, cpu);

		float get_pixel_coverage() restrict(amp, cpu);

		float get_hit_distance() restrict(amp, cpu);
	};
}