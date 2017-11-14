#pragma once

#include <math.h>
#include <vector>

using namespace std;

template <typename T>
class matrix
{
private: 
	T** _mat;

	int _row_size;
	int _col_size;
public:

	__declspec(dllexport) matrix() {}

	__declspec(dllexport) matrix(int row,int col, T init);


	__declspec(dllexport) matrix(T** mat,int row, int col);

	__declspec(dllexport) matrix(vector<vector<T>> mat);


	__declspec(dllexport) matrix(const matrix<T>& mat);


	__declspec(dllexport) matrix<T>& operator=(matrix<T> mat);

int get_row_length();


int get_col_length();


T at(int row, int col);


__declspec(dllexport) void set(int row, int col, T val);

~matrix();


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
	_row_size = row;
	_col_size = col;
	_mat = new T*[row];
	for (int i = 0; i < row; i++)
	{
		_mat[i] = new T[col];
	}
}

template<typename T>
matrix<T>::matrix(T** mat, int row, int col) : _mat(mat), _row_size(row), _col_size(col) {}

template<typename T>
matrix<T>::matrix(vector<vector<T>> mat)
{
	_row_size = mat.size();
	_col_size = mat[0].size();
	_mat = new T*[mat.size()];
	for (int i = 0; i < mat.size(); i++)
	{
		_mat[i] = new T[mat[i].size()];
		for (int j = 0; j < mat[i].size(); j++)
		{
			_mat[i][j] = mat[i][j];
		}
    }
}

template<typename T>
matrix<T>::matrix(const matrix<T>& mat)
{
	_row_size = mat._row_size;
	_col_size = mat._col_size;
	if (_row_size > 0)
	{
		_mat = new T*[_row_size];
		for (int i = 0; i < mat._row_size; i++)
		{
			_mat[i] = new T[_col_size];
			for (int j = 0; j < mat._col_size; j++)
			{
				_mat[i][j] = mat._mat[i][j];
			}
		}
	}
}


template<typename T>
matrix<T>& matrix<T>::operator =(matrix<T> mat)
{
	_row_size = mat._row_size;
	_col_size = mat._col_size;
	if (_row_size > 0)
	{
		_mat = new T*[_row_size];
		for (int i = 0; i < mat._row_size; i++)
		{
			_mat[i] = new T[_col_size];
			for (int j = 0; j < mat._col_size; j++)
			{
				_mat[i][j] = mat._mat[i][j];
			}
		}
	}
	return *this;
}

template<typename T>
matrix<T>::~matrix()
{
	if (_mat != nullptr)
	{
		for (int i = 0; i < _row_size; i++)
		{	
			
			delete[] _mat[i];
			_mat[i] = nullptr;
		}
		_mat = nullptr;
		
	}

}


template<typename T>
T matrix<T>::at(int row, int col)
{
	return _mat[row][col];
}



template<typename T>
void matrix<T>::set(int row, int col, T val)
{
	_mat[row][col] = val;
}

template <typename T>
int matrix<T>::get_row_length()
{
	return _row_size;
}

template <typename T>
int matrix<T>::get_col_length()
{
	return _col_size;
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




