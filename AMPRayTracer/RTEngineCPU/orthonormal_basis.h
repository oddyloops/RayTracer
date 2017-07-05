#pragma once

#include <vector>

using namespace std;

namespace rt_support
{
	class orthonormal_basis
	{
	private:
		vector<float> m_up_vector;
		vector<float> m_side_vector;
		vector<float> m_view_direction;

	public:
		orthonormal_basis();

		orthonormal_basis(vector<float> eye, vector<float> look_at, vector<float> up_direction);

		void  compute_orthonormal_basis(vector<float> eye, vector<float> look_at, vector<float> up_direction);

		vector<float> get_up_vector();

		vector<float> get_side_vector();

		vector<float> get_view_direction();

	};
}