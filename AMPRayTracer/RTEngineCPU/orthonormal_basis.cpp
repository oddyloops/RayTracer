#include "orthonormal_basis.h"
#include "vector_util.h"

using namespace rt_support;

orthonormal_basis::orthonormal_basis()
{

}

orthonormal_basis::orthonormal_basis(vector<float> eye, vector<float> look_at, vector<float> up_direction)
{
	compute_orthonormal_basis(eye, look_at, up_direction);
}

void  orthonormal_basis::compute_orthonormal_basis(vector<float> eye, vector<float> look_at, vector<float> up_direction)
{
	m_view_direction = look_at - eye;
	m_side_vector = vector_util::cross(m_view_direction, up_direction);
	m_up_vector = vector_util::cross(m_side_vector, m_view_direction);
	m_view_direction = vector_util::normalize(m_view_direction);
	m_up_vector = vector_util::normalize(m_up_vector);
	m_side_vector = vector_util::normalize(m_side_vector);
}

vector<float> orthonormal_basis::get_up_vector()
{
	return m_up_vector;
}

vector<float> orthonormal_basis::get_side_vector()
{
	return m_side_vector;
}

vector<float> orthonormal_basis::get_view_direction()
{
	return m_view_direction;
}