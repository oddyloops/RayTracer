#include "math_util.h"

float math_util::deg_to_rad(float deg)
{
	return deg * PI / 180;
}

float math_util::rad_to_degree(float rad)
{
	return rad * 180 / PI;
}