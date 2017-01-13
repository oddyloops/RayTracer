#pragma once
#define PI 3.14159265

class math_util
{

public:
	static float deg_to_rad(float deg) restrict(amp, cpu);

	static float rad_to_degree(float rad) restrict(amp, cpu);

	static float abs(float x) restrict(amp, cpu);
};