#include "rt_shader.h"

using namespace rt;

rt_shader::rt_shader()
{

}

vector<float> rt_shader::compute_shade(intersection_record rec, int generation)
{

	if (rec.get_geom_index() != -1)
	{
		float c = rec.get_geom_index() / 10.0;
		return{ c, c, c };
	}
	else {
		return pixel_data().get_default_color();
	}
}