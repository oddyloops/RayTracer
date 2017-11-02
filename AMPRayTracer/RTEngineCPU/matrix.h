#pragma once

#include <math.h>
#include <vector>

using namespace std;

template <typename T>
class matrix
{
private: 
	vector<vector<T>> _mat;
public:


	__declspec(dllexport) matrix(int row,int col, T init);


	__declspec(dllexport) matrix(vector<vector<T>> mat);


	int get_row_length();


	int get_col_length();


	T at(int row, int col);

	__declspec(dllexport) vector<T> get_row(int row);

	__declspec(dllexport) vector<T> get_col(int col);

	__declspec(dllexport) void set(int row, int col, T val);


	static bool equal_dimensions(matrix<float>& lhs, matrix<float>& rhs)
	{
		return lhs.get_row_length() == rhs.get_row_length()
			&& lhs.get_col_length() == rhs.get_col_length();
	}


	static matrix<float> rotate_x(float angle_rad)
	{
		matrix<float> xform({ { 1, 0, 0 },{ 0, cosf(angle_rad), -sinf(angle_rad) },{ 0, sinf(angle_rad), cosf(angle_rad) } });
		return xform;
	}


	static matrix<float> rotate_y(float angle_rad)
	{

		matrix<float> xform({ { cosf(angle_rad), 0, sinf(angle_rad) },{ 0, 1, 0 },{ -sinf(angle_rad), 0, cosf(angle_rad) } });
		return xform;
	}


	static matrix<float> rotate_z(float angle_rad)
	{
		vector<vector<float>> args = { { cosf(angle_rad), sinf(angle_rad) , 0 },{ -sinf(angle_rad), cosf(angle_rad), 0 },{ 0,0,1 } };
		matrix<float> xform(args);
		return xform;
	}


	static matrix<float> scale(vector<float> scl_vec)
	{

		matrix<float> xform({ { scl_vec[0],0,0 },{ 0,scl_vec[1],0 },{ 0,0,scl_vec[2] } });
		return xform;
	}



	static matrix<float> translate_from_vector(vector<float> translate)
	{
		//using a 3x3 matrix instead of a 4x4 homogeinuous matrix using a systematic division to reduce z(1),z(2),x(3) to 1 respectively
		matrix<float> xform({ { 1,0,translate[0] },{ 0,1,translate[1] },{ translate[2],0,1 } });
		return xform;
	}

	static vector<float> transform(vector<float> vec, matrix<float> trans_mat)
	{
		float temp[] = { vec[0],vec[1], vec[2] };
		vector<float> result = { 0,0,0 };
		for (int i = 0; i < 3; i++)
		{
			int start = i * 3;
			int end = start + 3;
			for (int j = start; j < end; j++)
			{

				result[i] += trans_mat.at(i, j % 3) * temp[j % 3];
			}
		}
		return result;
	}



	



	



};



template<typename T>
matrix<T>::matrix(int row, int col, T init)
{
	for (int i = 0; i < row; i++)
	{
		vector<float> row_vec(col);
		for (int j = 0; j < col; j++)
		{
			row_vec.push_back(init);
		}
		_mat.push_back(row_vec);
	}
}

template<typename T>
matrix<T>::matrix(vector<vector<T>> mat) : _mat(mat) {}

template<typename T>
T matrix<T>::at(int row, int col)
{
	return _mat[row][col];
}

template<typename T>
vector<T> matrix<T>::get_row(int row)
{
	return _mat[row];
}

template<typename T>
vector<T> matrix<T>::get_col(int col)
{

	vector<T> col_vec;
	for (int i = 0; i < _mat.size(); i++)
	{
		col_vec.push_back(_mat[i][col]);
	}
	return col_vec;
}

template<typename T>
void matrix<T>::set(int row, int col, T val)
{
	_mat[row][col] = val;
}

template <typename T>
int matrix<T>::get_row_length()
{
	return _mat.size();
}

template <typename T>
int matrix<T>::get_col_length()
{
	return _mat[0].size();
}



static matrix<float> operator+(matrix<float>& lhs, matrix<float>& rhs)
{
	if (!matrix<float>::equal_dimensions(lhs, rhs))
	{
		throw exception("matrices are of different dimensions");
	}

	matrix<float> result(lhs.get_row_length(), lhs.get_col_length(), 0);
	for (int i = 0; i < lhs.get_row_length(); i++)
	{
		for (int j = 0; j < lhs.get_col_length(); j++)
		{
			result.set(i, j, lhs.at(i, j) + rhs.at(i, j));
		}

	}
	return result;
}
static matrix<float> operator*(matrix<float>& lhs, matrix<float>& rhs)
{
	if (lhs.get_col_length() != rhs.get_row_length())
	{
		throw exception("invalid matrix multiplication");
	}
	matrix<float> result(lhs.get_col_length(), rhs.get_row_length(), 0);
	int result_size = result.get_col_length() * result.get_row_length();
	for (int i = 0; i < result_size; i++)
	{
		float entry = 0;
		for (int j = 0; j < lhs.get_col_length(); j++)
		{

			for (int k = 0; k < rhs.get_row_length(); k++)
			{
				entry += lhs.at(j, k)  * rhs.at(k, j);
			}
		}
		result.set(i / result.get_row_length(), i %  result.get_row_length(), entry);
	}
	return result;
}




