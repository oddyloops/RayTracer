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
	result.push_back((lhs[0] * rhs[1]) - (lhs[1] * rhs[0]));
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

bool vector_util::is_mirror_of(vector<float> a, vector<float> b)
{
	if (a.size() != b.size())
	{
		throw exception("Cannot use vectors of different dimensions");
	}

	for (int i = 0; i < a.size(); i++)
	{
		//their absolute values are not the same or their values are the same
		if ((abs(a[i]) - abs(b[i]) > FLT_EPSILON) || ( a[i]- b[i]  <= FLT_EPSILON))
		{
			return false;
		}
	}

	return true;
}


float vector_util::point_line_distance(vector<float> pt, vector<float> ln_start, vector<float> ln_end)
{
	//compute distance squared between point and line start
	vector<float> p_start = pt - ln_start;
	float p_start_dist = vector_util::magnitude_sqr(p_start);

	//project p_start on line and get the magnitude square
	float p_start_line = powf(vector_util::dot(p_start, vector_util::normalize(ln_end - ln_start)),2);

	//use pythagoras theorem to return shortest distance between pt and line
	return sqrtf(p_start_dist - p_start_line);


}


vector<float> vector_util::clip_color(vector<float> input)
{
	vector<float> result;
	for (float c : input) 
	{
		result.push_back(fmaxf(0,fminf(c, 1.0f)));
	}
	return result;
}

bool vector_util::is_zero(vector<float> v)
{
	for (float f : v)
	{
		if (abs(f) > FLT_EPSILON)
		{
			return false;
		}
	}

	return true;
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


vector<float> operator*(vector<float>& lhs, vector<float>& rhs)
{
	if (lhs.size() != rhs.size())
	{
		throw exception("Vectors are of different dimensions");
	}

	vector<float> result;

	for (int i = 0; i < lhs.size(); i++)
	{
		result.push_back(lhs[i] * rhs[i]);
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

