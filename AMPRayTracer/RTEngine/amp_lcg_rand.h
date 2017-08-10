#pragma once
#include <amp.h>
using namespace concurrency;

///<summary>
///Pseudo RNG for random super sampling in the GPU code
///</summary>
class amp_lcg_rand
{
private:
	unsigned int m_state;
	unsigned int m_max_int;
public:
	amp_lcg_rand() restrict(amp, cpu) {/*default constructor*/}

	amp_lcg_rand(unsigned int seed) restrict(amp,cpu);

	float rand_f() restrict(amp,cpu);

	int rand(int min, int max) restrict(amp,cpu);

};