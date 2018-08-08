#include <amp_math.h>
#include "vector_amp.h"

using namespace concurrency::fast_math;


float vector_amp::magnitude_sqr(float_2 input) restrict(amp,cpu)
{
	return input.r * input.r + input.g * input.g ;
}


float vector_amp::magnitude_sqr(float_3 input) restrict(amp,cpu)
{
	return input.r * input.r + input.g * input.g + input.b * input.b;
}

float vector_amp::magnitude(float_2 input) restrict(amp,cpu)
{
	return sqrtf(magnitude_sqr(input));
}

float vector_amp::magnitude(float_3 input) restrict(amp,cpu)
{
	return sqrtf(magnitude_sqr(input));
}

float_2 vector_amp::normalize(float_2 input) restrict(amp,cpu)
{
	return input / magnitude(input);
}

float_3 vector_amp::normalize(float_3 input) restrict(amp,cpu)
{
	return input / magnitude(input);
}


float vector_amp::dot(float_3 lhs, float_3 rhs) restrict(amp,cpu)
{
	return lhs.r * rhs.r + lhs.g * rhs.g + lhs.b * rhs.b;
}

float vector_amp::dot(float_2 lhs, float_2 rhs) restrict(amp,cpu)
{
	return lhs.r * rhs.r + lhs.g * rhs.g;
}

float_3 vector_amp::cross(float_3 lhs, float_3 rhs) restrict(amp, cpu)
{
	float_3 result;
	result.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
	result.y = -((lhs.x * rhs.z) - (lhs.z * rhs.x));
	result.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);
	return result;
}


float vector_amp::point_line_distance(float_3 pt, float_3 ln_start, float_3 ln_end) restrict(amp, cpu)
{
	float t = -(dot(ln_start - pt, ln_end - ln_start)) / magnitude_sqr(ln_end - ln_start);

	float dist = powf((ln_start.x - pt.x) + (t * (ln_end.x - ln_start.x)), 2)
		+ powf((ln_start.y - pt.y) + (t * (ln_end.y - ln_start.y)), 2)
		+ powf((ln_start.z - pt.z) + (t * (ln_end.z - ln_start.z)), 2);

	return sqrtf(dist);
}

float_3 vector_amp::clip_color(float_3 color) restrict(amp)
{
	float_3 result;
	result.r = fmaxf(0,fminf(color.r, 1.0f));
	result.g = fmaxf(0,fminf(color.g, 1.0f));
	result.b = fmaxf(0,fminf(color.b, 1.0f));
	return result;
}