#include <amp_math.h>

#include "rt_wave_props.h"
#include "vector_amp.h"

using namespace concurrency::fast_math;
using namespace rt;

float_3 rt_wave_props::reflect(float_3 normal, float_3 light_dir) restrict(amp)
{
	return vector_amp::normalize(-(2 * (vector_amp::dot(normal, light_dir)) * normal) + light_dir);
}

float_3 rt_wave_props::refract(float_3 normal, float_3 light_dir, float src_refr_index, float dest_refr_index) restrict(amp)
{
	float n = dest_refr_index / src_refr_index;
	float_3 v = vector_amp::normalize(-light_dir);
	float cosThetaI = vector_amp::dot(v, normal);
	float invN = 1 / n;
	float cosThetaT = sqrtf(1 - ((invN * invN) * (1 - (cosThetaI * cosThetaI))));
	float_3 refracted =-((invN * v) + ((cosThetaT - (invN * cosThetaI)) * normal));
	return refracted;
}