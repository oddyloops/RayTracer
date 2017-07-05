#include "rt_gateway.h"

using namespace rt_support::geometries;

int main()
{
	rt_sphere sph1 = rt_sphere(float_3(-4, 0, 2), 1);
	rt_sphere sph2 = rt_sphere(float_3(-1, 1.5, 0), 1.5);
	rt_sphere sph3 = rt_sphere(float_3(1, 1, 1), 2);
	rt_sphere sph4 = rt_sphere(float_3(2, 3, -1), 0.5);
	vector<rt_sphere> spheres = {sph1,sph2,sph3,sph4};

	float_3 vtx1[] = { float_3(0,1,-2), float_3(2,1,-2),float_3(2,1,2),float_3(0,1,2) };
	rt_rectangle rect1 = rt_rectangle(vtx1);
	float_3 vtx2[] = { float_3(-4,0,-4), float_3(4,0,-4),float_3(4,0,4),float_3(-4,0,4) };
	rt_rectangle rect2 = rt_rectangle(vtx2);
	float_3 vtx3[] = { float_3(-4,-2,4), float_3(4,-2,4),float_3(4,2,4),float_3(-4,2,4) };
	rt_rectangle rect3 = rt_rectangle(vtx3);
	vector<rt_rectangle> rectangles = { rect1,rect2,rect3 };

	rt_camera cam = rt_camera(float_3(-8, 5, -8), float_3(0, 0, 0), float_3(0, 1, 0), 45, 2);
	image_spec spec = image_spec(1280, 720, 5);
	for (int i = 0; i < 10; i++)
	{
		auto results = rt_gateway::ray_trace(spheres, rectangles, vector<rt_material>(), cam, spec);
	}
	system("PAUSE");
}