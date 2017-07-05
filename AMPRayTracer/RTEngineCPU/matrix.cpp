#include "matrix.h"

matrix::matrix(int row, int col, float init) 
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

matrix::matrix(vector<vector<float>> mat) : _mat(mat) {}

float matrix::at(int row, int col) 
{
	return _mat[row][col];
}

vector<float> matrix::get_row(int row)
{
	return _mat[row];
}

vector<float> matrix::get_col(int col) 
{

	vector<float> col_vec;
	for (int i = 0; i < _mat.size(); i++) 
	{
		col_vec.push_back(_mat[i][col]);
	}
	return col_vec;
}

void matrix::set(int row, int col, float val) 
{
	_mat[row][col] = val;
}

int matrix::get_row_length()
{
	return _mat.size();
}

int matrix::get_col_length()
{
	return _mat[0].size();
}


bool matrix::equal_dimensions(matrix lhs, matrix rhs)
{
	return lhs.get_row_length() == rhs.get_row_length()
		&& lhs.get_col_length() == rhs.get_col_length();
}


matrix matrix::rotate_x(float angle_rad)
{
	matrix xform({ { 1, 0, 0},{ 0, cosf(angle_rad), -sinf(angle_rad)},{ 0, sinf(angle_rad), cosf(angle_rad)} });
	return xform;
}

matrix matrix::rotate_y(float angle_rad)
{
	matrix xform({ { cosf(angle_rad), 0, sinf(angle_rad)},{ 0, 1, 0},{ -sinf(angle_rad), 0, cosf(angle_rad)} });
	return xform;
}

matrix matrix::rotate_z(float angle_rad)
{
	matrix xform({ { cosf(angle_rad), sinf(angle_rad) , 0},{ -sinf(angle_rad), cosf(angle_rad), 0},{ 0,0,1} });
	return xform;
}

matrix matrix::scale(vector<float> scl_vec)
{

	matrix xform({ { scl_vec[0],0,0},{0,scl_vec[1],0},{0,0,scl_vec[2]} });
	return xform;
}

matrix matrix::translate_from_vector(vector<float> translate)
{
	//using a 3x3 matrix instead of a 4x4 homogeinuous matrix using a systematic division to reduce z(1),z(2),x(3) to 1 respectively
	matrix xform ({ { 1,0,translate[0]},{0,1,translate[1]},{translate[2],0,1 } });
	return xform;
}

vector<float> matrix::transform(vector<float> vec, matrix trans_mat)
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



matrix operator+(matrix& lhs, matrix& rhs)
{
	if (!matrix::equal_dimensions(lhs, rhs))
	{
		throw exception("matrices are of different dimensions");
	}

	matrix result(lhs.get_row_length(),lhs.get_col_length(), 0);
	for (int i = 0; i < lhs.get_row_length(); i++)
	{
		for (int j = 0; j < lhs.get_col_length(); j++)
		{
			result.set(i, j, lhs.at(i, j) + rhs.at(i, j));
		}
		
	}
	return result;
}



matrix operator*(matrix&lhs, matrix& rhs)
{
	if (lhs.get_col_length() != rhs.get_row_length())
	{
		throw exception("invalid matrix multiplication");
	}
	matrix result(lhs.get_col_length(), rhs.get_row_length(), 0);
	int result_size = result.get_col_length() * result.get_row_length();
	for(int i = 0; i < result_size; i++)
	{
		float entry = 0;
		for (int j = 0; j < lhs.get_col_length(); j++)
		{
			
			for (int k = 0; k < rhs.get_row_length(); k++)
			{
				entry += lhs.at(j, k)  * rhs.at(k, j);
			}
		}
		result.set(i / result.get_row_length(), i %  result.get_row_length(),entry);
	}
	return result;
}


