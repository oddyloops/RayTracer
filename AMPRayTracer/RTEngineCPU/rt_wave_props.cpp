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
	vector<float> v = vector_util::negate(light_dir);
	float nDotV = vector_util::dot(normal, v);

	vector<float> refracted = (
		n * nDotV - sqrtf(
			1 - (
				n*n*(
					1- 
					(
						nDotV * nDotV))))) * normal  - (
							n * v);
	return refracted;

}