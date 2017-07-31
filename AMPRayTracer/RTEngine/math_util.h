#pragma once
#define PI 3.14159265f
#include "vector_amp.h"
#include <amp_math.h>

using namespace concurrency::fast_math;
class math_util
{

public:
	static float deg_to_rad(float deg) restrict(amp, cpu);

	static float rad_to_degree(float rad) restrict(amp, cpu);

	static float abs(float x) restrict(amp, cpu);

	static float clock_wise_angle(float_3 v1, float_3 v2, float_3 normal) restrict(amp);
};