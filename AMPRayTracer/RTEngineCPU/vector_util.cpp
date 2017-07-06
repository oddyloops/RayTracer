#include "vector_util.h"


float vector_util::magnitude_sqr(vector<float> input)
{
	float result = 0;
	for (int i = 0; i < input.size(); i++)
	{
		result += (input[i] * input[i]);
	}
	return result;
}




float vector_util::magnitude(vector<float> input)
{
	return sqrtf(magnitude_sqr(input));
}


vector<float> vector_util::normalize(vector<float> input)
{
	float magnitude = vector_util::magnitude(input);
	for (int i = 0; i < input.size(); i++) 
	{
		input[i] /= magnitude;
	}
	return input;
}



float vector_util::dot(vector<float> lhs, vector<float> rhs)
{
	if (lhs.size() != rhs.size())
	{
		throw exception("Cannot dot vectors of different dimensions");
	}
	return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}


vector<float> vector_util::cross(vector<float> lhs, vector<float> rhs)
{
	if(lhs.size() != 3 || rhs.size() != 3)
	{
		throw exception("Both vectors must be 3-dimensional");
	}
	vector<float> result;
	result.push_back( (lhs[1] * rhs[2]) - (lhs[2] * rhs[1]));
	result.push_back(-((lhs[0] * rhs[2]) - (lhs[2] * rhs[0])));
	result.push_back((lhs[0] * rhs[1]) + (lhs[1] * rhs[0]));
	return result;
}


vector<float> vector_util::negate(vector<float> input)
{
	vector<float> result;
	for (float& item : input)
	{
		result.push_back(-item);
	}

	return result;
}




vector<float> operator+(vector<float>& lhs, vector<float>& rhs)
{
	if (lhs.size() != rhs.size())
	{
		throw exception("Vectors are of different dimensions");
	}

	vector<float> result;

	for (int i = 0; i < lhs.size(); i++)
	{
		result.push_back(lhs[i] + rhs[i]);
	}

	return result;
}


vector<float> operator-(vector<float>& lhs, vector<float>& rhs)
{
	if (lhs.size() != rhs.size())
	{
		throw exception("Vectors are of different dimensions");
	}

	vector<float> result;

	for (int i = 0; i < lhs.size(); i++)
	{
		result.push_back(lhs[i] - rhs[i]);
	}

	return result;
}


vector<float> operator*(float lhs, vector<float> rhs)
{
	vector<float> result;

	for (float& item : rhs)
	{
		result.push_back(item * lhs);
	}

	return result;
}

vector<float> operator*(vector<float> lhs, float rhs)
{
	vector<float> result;

	for (float& item : lhs)
	{
		result.push_back(item * rhs);
	}

	return result;
}

vector<float> operator/(vector<float>& lhs, float& rhs)
{
	vector<float> result;

	for (float& item : lhs)
	{
		result.push_back(item/rhs);
	}

	return result;
}


vector<float> operator/(vector<float> lhs, int rhs)
{
	vector<float> result;

	for (float& item : lhs)
	{
		result.push_back(item / rhs);
	}

	return result;
}