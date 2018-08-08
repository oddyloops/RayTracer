#include "amp_lcg_rand.h"

amp_lcg_rand::amp_lcg_rand(unsigned int seed) restrict(amp, cpu)
{
	m_state = seed;
	m_max_int = (1 << 32) - 1;
}

float amp_lcg_rand::rand_f() restrict(amp, cpu)
{
	m_state = m_state * 1664525 + 1013904223;
	return static_cast<float>(m_state) / m_max_int;
}

int amp_lcg_rand::rand(int min, int max) restrict(amp, cpu)
{
	m_state = m_state * 1664525 + 1013904223;
	return min + (m_state % (max - min));
}
