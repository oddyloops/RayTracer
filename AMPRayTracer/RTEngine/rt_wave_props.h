#pragma once
#include <amp.h>
#include <amp_graphics.h>


using namespace concurrency;
using namespace concurrency::graphics;

namespace rt
{
	class rt_wave_props
	{
	public:
		static float_3 reflect(float_3 normal, float_3 light_dir) restrict(amp);

		static float_3 refract(float_3 normal, float_3 light_dir, float src_refr_index, float dest_refr_index) restrict(amp);
	};
}