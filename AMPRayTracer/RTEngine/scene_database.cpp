#include "scene_database.h"

using namespace rt_support;

scene_database::scene_database(std::vector<rt_sphere> spheres, std::vector<rt_rectangle> rects) restrict(amp, cpu)
{
	m_spheres(spheres);

}

void add_sphere(rt_sphere s) restrict(amp, cpu);

void add_rect(rt_rectangle r) restrict(amp, cpu);

rt_sphere get_sphere(int index) restrict(amp, cpu);

rt_rectangle get_rect(int index) restrict(amp, cpu);

array_view<rt_sphere, 1> get_all_spheres() restrict(amp, cpu);

array_view<rt_rectangle, 1> get_all_rects() restrict(amp, cpu);

int get_num_spheres() restrict(amp, cpu);

int get_num_rects() restrict(amp, cpu);