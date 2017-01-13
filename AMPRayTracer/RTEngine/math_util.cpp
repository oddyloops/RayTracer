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