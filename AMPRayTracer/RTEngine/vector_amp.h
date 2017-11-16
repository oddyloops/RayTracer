#pragma once
#include <amp.h>
#include <amp_graphics.h>
using namespace concurrency;
using namespace concurrency::graphics;

///<summary>
///Contains all the vector manipulation helper functions not present in the amp_graphics header
///</summary>
class vector_amp
{
public:

	static float magnitude_sqr(float_2 input) restrict(amp,cpu);

	static float magnitude_sqr(float_3 input) restrict(amp,cpu);

	static float magnitude(float_2 input) restrict(amp,cpu);

	static float magnitude(float_3 input) restrict(amp,cpu);

	static float_2 normalize(float_2 input) restrict(amp,cpu);

	static float_3 normalize(float_3 input) restrict(amp,cpu);

	static float dot(float_3 lhs, float_3 rhs) restrict(amp,cpu);

	static float dot(float_2 lhs, float_2 rhs) restrict(amp,cpu);

	static float_3 cross(float_3 lhs, float_3 rhs) restrict(amp, cpu);

	static int is_zero(float_3 v) restrict(amp, cpu);

	static int is_mirror_of(float_3 lhs, float_3 rhs) restrict(amp,cpu);

	///<summary>
	///computes distance between a point and a line assuming they are on the same plane
	///</summary>
	static float point_line_distance(float_3 pt, float_3 ln_start, float_3 ln_end) restrict(amp,cpu);

	static float_3 clip_color(float_3 color) restrict(amp);
};