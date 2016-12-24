#include <amp_math.h>
#include "vector_amp.h"

using namespace concurrency::fast_math;


float vector_amp::magnitude_sqr(float_2 input) restrict(amp) restrict(cpu)
{
	return input.r * input.r + input.g * input.g ;
}


float vector_amp::magnitude_sqr(float_3 input) restrict(amp) restrict(cpu)
{
	return input.r * input.r + input.g * input.g + input.b * input.b;
}

float vector_amp::magnitude(float_2 input) restrict(amp) restrict(cpu)
{
	return sqrtf(magnitude_sqr(input));
}

float vector_amp::magnitude(float_3 input) restrict(amp) restrict(cpu)
{
	return sqrtf(magnitude_sqr(input));
}

float_2 vector_amp::normalize(float_2 input) restrict(amp) restrict(cpu)
{
	return input / magnitude(input);
}

float_3 vector_amp::normalize(float_3 input) restrict(amp) restrict(cpu)
{
	return input / magnitude(input);
}


float vector_amp::dot(float_3 lhs, float_3 rhs) restrict(amp) restrict(cpu)
{
	return lhs.r * rhs.r + lhs.g * rhs.g + lhs.b * rhs.b;
}

float vector_amp::dot(float_2 lhs, float_2 rhs) restrict(amp) restrict(cpu)
{
	return lhs.r * rhs.r + lhs.g * rhs.g;
}