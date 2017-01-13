#pragma once
#include <amp.h>
#include <amp_graphics.h>

using namespace concurrency;
using namespace concurrency::graphics;

namespace rt_support
{
	class orthonormal_basis
	{
	private:
		float_3 m_up_vector;
		float_3 m_side_vector;
		float_3 m_view_direction;

	public:
		orthonormal_basis() restrict(amp, cpu) { /*default*/ }

		orthonormal_basis(float_3 eye, float_3 look_at, float_3 up_direction) restrict(cpu,amp);

		void  compute_orthonormal_basis(float_3 eye, float_3 look_at, float_3 up_direction) restrict(cpu, amp);

		float_3 get_up_vector() restrict(amp,cpu);

		float_3 get_side_vector() restrict(amp, cpu);

		float_3 get_view_direction() restrict(amp, cpu);

	};
}