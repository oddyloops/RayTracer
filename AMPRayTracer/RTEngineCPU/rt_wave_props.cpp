#include "rt_wave_props.h"
#include "vector_util.h"

using namespace rt;

vector<float> rt_wave_props::reflect(vector<float> normal, vector<float> light_dir)
{
	return vector_util::normalize(2 * (vector_util::dot(normal, light_dir)) * normal - light_dir);
}