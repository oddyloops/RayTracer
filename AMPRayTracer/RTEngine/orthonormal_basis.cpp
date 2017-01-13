#include "orthonormal_basis.h"
#include "vector_amp.h"

using namespace rt_support;

orthonormal_basis::orthonormal_basis(float_3 eye, float_3 look_at, float_3 up_direction) restrict(cpu, amp)
{
	compute_orthonormal_basis(eye, look_at, up_direction);
}

void  orthonormal_basis::compute_orthonormal_basis(float_3 eye, float_3 look_at, float_3 up_direction) restrict(cpu, amp)
{
	m_view_direction = look_at - eye;
	m_side_vector = vector_amp::cross(m_view_direction, up_direction);
	m_up_vector = vector_amp::cross(m_side_vector, m_view_direction);
	m_view_direction = vector_amp::normalize(m_view_direction);
	m_up_vector = vector_amp::normalize(m_up_vector);
	m_side_vector = vector_amp::normalize(m_side_vector);
}

float_3 orthonormal_basis::get_up_vector() restrict(amp, cpu)
{
	return m_up_vector;
}

float_3 orthonormal_basis::get_side_vector() restrict(amp, cpu)
{
	return m_side_vector;
}

float_3 orthonormal_basis::get_view_direction() restrict(amp, cpu)
{
	return m_view_direction;
}