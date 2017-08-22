#pragma once

#include <vector>

using namespace std;

class vector_util
{
public:

	static float magnitude_sqr(vector<float> input);


	static float magnitude(vector<float> input);

	static vector<float> normalize(vector<float> input);

	static float dot(vector<float> lhs, vector<float> rhs);

	static vector<float> cross(vector<float> lhs, vector<float> rhs);

	static vector<float> negate(vector<float> input);

	static vector<float> clip_color(vector<float> input);

	///<summary>
	///computes distance between a point and a line assuming they are on the same plane
	///</summary>
	static float point_line_distance(vector<float> pt, vector<float> ln_start, vector<float> ln_end);

};

vector<float> operator+(vector<float>& lhs, vector<float>& rhs);

vector<float> operator-(vector<float>& lhs, vector<float>& rhs);

vector<float> operator*(vector<float>& lhs, vector<float>& rhs);

vector<float> operator*(float lhs, vector<float> rhs);

vector<float> operator*(vector<float> lhs, float rhs);

vector<float> operator/(vector<float>& lhs, float& rhs);

vector<float> operator/(vector<float> lhs, int rhs);

