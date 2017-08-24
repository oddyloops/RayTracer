#include "rt_wave_props.h"
#include "vector_amp.h"

using namespace rt;

float_3 rt_wave_props::reflect(float_3 normal, float_3 light_dir) restrict(amp)
{
	return vector_amp::normalize(-(2 * (vector_amp::dot(normal, light_dir)) * normal) + light_dir);
}