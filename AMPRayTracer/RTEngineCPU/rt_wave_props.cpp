#include "rt_wave_props.h"
#include "vector_util.h"

using namespace rt;

vector<float> rt_wave_props::reflect(vector<float> normal, vector<float> light_dir)
{
	return vector_util::normalize((-2 * (vector_util::dot(normal, light_dir)) * normal) + light_dir);
}

vector<float> rt_wave_props::refract(vector<float> normal, vector<float> light_dir, float src_refr_index, float dest_refr_index)
{
	float n = dest_refr_index / src_refr_index;
	vector<float> v = vector_util::normalize(vector_util::negate(light_dir));
	float cosThetaI = vector_util::dot(v, normal);
	float invN = 1 / n;
	float cosThetaT = sqrtf(1 - ((invN * invN) * (1 - (cosThetaI * cosThetaI))));
	vector<float> refracted = vector_util::negate(((invN * v) + ((cosThetaT - (invN * cosThetaI)) * normal)));
	return refracted;


}