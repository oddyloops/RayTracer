#pragma once
#define PI 3.14159265f
#include "vector_amp.h"
#include <amp_math.h>

using namespace concurrency::fast_math;
///<summary>
///Provides helper math functions
///</summary>

class math_util
{

public:
	static float deg_to_rad(float deg) restrict(amp, cpu);

	static float rad_to_degree(float rad) restrict(amp, cpu);

	static float abs(float x) restrict(amp, cpu);
	
	///<summary>
	///Computes the angle between two vectors (v1,v2) in the clock-wise direction
	///The normal is used to compute the determinant of the v1, v2, normal matrix
	///</summary>
	static float clock_wise_angle(float_3 v1, float_3 v2, float_3 normal) restrict(amp);
};