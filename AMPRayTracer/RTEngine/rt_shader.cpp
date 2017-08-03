#include "rt_shader.h"

using namespace rt;

rt_shader::rt_shader() restrict(amp, cpu)
{

}

float_3 rt_shader::compute_shade(intersection_record rec, int generation) restrict(amp)
{
	if (rec.get_geom_index() != -1)
	{
		float c = rec.get_geom_index() / 10.0f;
		return float_3(c,c,c);
	}
	else {
		return pixel_data().get_default_color();
	}
}