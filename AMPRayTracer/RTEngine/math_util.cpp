#include "math_util.h"


float math_util::deg_to_rad(float deg) restrict(amp, cpu)
{
	return deg * PI / 180;
	
}
 
float math_util::rad_to_degree(float rad) restrict(amp, cpu)
{
	return rad * 180 / PI;
}

float math_util::abs(float x) restrict(amp,cpu)
{
	if (x < 0)
		return -x;
	return x;
}


float math_util::clock_wise_angle(float_3 v1, float_3 v2, float_3 normal) restrict(amp)
{
	float dot = vector_amp::dot(v1, v2);
	float det = v1.x * v2.y * normal.z
		+ v2.x * normal.y * v1.z
		+ normal.x * v1.y * v2.z
		- v1.z * v2.y * normal.x
		- v2.z * normal.y * v1.x
		- normal.z * v1.y * v2.x;
	return atan2f(det, dot);
}
