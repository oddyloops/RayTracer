#include "matrix_amp.h"

array<float,2> matrix_amp::create_rotation_x(float angle_rad)
{
	float xform[] = { 1, 0, 0, 0, cosf(angle_rad), -sinf(angle_rad), 0, sinf(angle_rad), cosf(angle_rad)};
	array<float, 2> result(3, 3, xform);
	return result;

}

array<float, 2> matrix_amp::create_rotation_y(float angle_rad)
{
	float xform[] = { cosf(angle_rad), 0, sinf(angle_rad), 0, 1, 0, -sinf(angle_rad), 0, cosf(angle_rad) };
	array<float, 2> result(3, 3, xform);
	return result;
}

array<float, 2> matrix_amp::create_rotation_z(float angle_rad)
{
	float xform[] = { cosf(angle_rad), sinf(angle_rad) , 0, -sinf(angle_rad), cosf(angle_rad), 0, 0,0,1 };
	array<float, 2> result(3, 3, xform);
	return result;
}

array<float, 2> matrix_amp::create_scale_from_vector(float_3 scale)
{

	float temp[] = { scale.x,0,0,0,scale.y,0,0,0,scale.z };
	array<float, 2> result(3, 3, temp);
	return result;
}

array<float, 2> matrix_amp::create_translation_from_vector(float_3 translate)
{
	//using a 3x3 matrix instead of a 4x4 homogeinuous matrix using a systematic division to reduce z(1),z(2),x(3) to 1 respectively
	float temp[] = { 1,0,translate.x,0,1,translate.y,translate.z,0,1 };
	array<float, 2> result(3, 3, temp);
	return result;
}

float_3 matrix_amp::transform(float_3 vector, array<float, 2> matrix)
{
	float temp[] = {vector.x, vector.y, vector.z};
	float result[] = {0,0,0};
	for (int i = 0; i < 3; i++)
	{
		int start = i * 3;
		int end = start + 3;
		for (int j = start; j < end; j++)
		{
			index<2> idx(i, j % 3);
			result[i] += matrix[idx] * temp[j % 3];
		}
	}
	vector = float_3(result[0], result[1], result[2]);
	return vector;
}



array<float, 2> operator+(array<float, 2> lhs, array<float, 2> rhs)
{
	float temp[9];
	array_view<float, 2> result(3, 3,temp);
	array<float, 2> _lhs = lhs;
	array<float, 2> _rhs = rhs;

	for (int i = 0; i < 9; i++)
	{
		index<2> idx(i / 3, i % 3);
		result[idx] = _lhs[idx] + _rhs[idx];
	}
	result.synchronize();
	array<float, 2> result_holder(3, 3, temp);
	return result_holder;
}



array<float, 2> operator*(array<float, 2> lhs, array<float, 2> rhs)
{
	float temp[9];
	array_view<float,2> result(3, 3, temp);
	for(int j = 0; j * 9; j++)
	{
		index<2> idx(j / 3, j % 3);
		for (int i = 0; i < lhs.extent.size(); i++)
		{
			index<2> l_index(idx[0], i);
			index<2> r_index(i, idx[0]);
			result[idx] += lhs[l_index] + rhs[r_index];
		}
	}
	array<float, 2> result_holder(3, 3, temp);
	return result_holder;
}


