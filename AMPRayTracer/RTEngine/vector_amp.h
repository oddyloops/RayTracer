#pragma once
#include <amp.h>
#include <amp_graphics.h>
using namespace concurrency;
using namespace concurrency::graphics;

class vector_amp
{
public:

	static float magnitude_sqr(float_2 input) restrict(amp) restrict(cpu);

	static float magnitude_sqr(float_3 input) restrict(amp) restrict(cpu);

	static float magnitude(float_2 input) restrict(amp) restrict(cpu);

	static float magnitude(float_3 input) restrict(amp) restrict(cpu);

	static float_2 normalize(float_2 input) restrict(amp) restrict(cpu);

	static float_3 normalize(float_3 input) restrict(amp) restrict(cpu);

	static float dot(float_3 lhs, float_3 rhs) restrict(amp) restrict(cpu);

	static float dot(float_2 lhs, float_2 rhs) restrict(amp) restrict(cpu);
};