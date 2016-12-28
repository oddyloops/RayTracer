#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include <math.h>

using namespace concurrency;
using namespace concurrency::graphics;

class matrix_amp
{
public:
	static array<float,2> create_rotation_x(float angle_rad);

	static array<float,2> create_rotation_y(float angle_rad);

	static array<float,2> create_rotation_z(float angle_rad);

	static array<float, 2> create_scale_from_vector(float_3 scale);

	static array<float, 2> create_translation_from_vector(float_3 translate);

};

array<float,2> operator+(array<float,2> lhs, array<float,2> rhs);

array<float,2> operator*(array<float,2> lhs, array<float,2> rhs);
