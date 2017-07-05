#pragma once

#include <math.h>
#include <vector>

using namespace std;

class matrix
{
private: 
	vector<vector<float>> _mat;
public:
	matrix(int row,int col, float init);

	matrix(vector<vector<float>> mat);

	int get_row_length();

	int get_col_length();

	float at(int row, int col);

	vector<float> get_row(int row);

	vector<float> get_col(int col);

	void set(int row, int col, float val);

	static bool equal_dimensions(matrix lhs, matrix rhs);

	static matrix rotate_x(float angle_rad);

	static matrix rotate_y(float angle_rad);

	static matrix rotate_z(float angle_rad);

	static matrix scale(vector<float> scl_vec);

	static matrix translate_from_vector(vector<float> translate);

	static vector<float> transform(vector<float> vec, matrix trans_mat);

};

matrix operator+(matrix& lhs, matrix& rhs);

matrix operator*(matrix& lhs, matrix& rhs);
