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

};

static vector<float> operator+(vector<float>& lhs, vector<float>& rhs);

static vector<float> operator-(vector<float>& lhs, vector<float>& rhs);

static vector<float> operator*(float lhs, vector<float> rhs);

static vector<float> operator*(vector<float> lhs, float rhs);

static vector<float> operator/(vector<float>& lhs, float& rhs);

static vector<float> operator/(vector<float> lhs, int rhs);