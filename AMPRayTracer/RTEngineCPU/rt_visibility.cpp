#include "rt_visibility.h"

using namespace rt;


rt_visibility::rt_visibility()
{

}

void rt_visibility::compute_visibility(ray r, int except_geom_index, intersection_record& rec, vector<rt_sphere> spheres, vector<rt_rectangle> rectangles)
{
	for (int i = 0; i < spheres.size(); i++)
	{
		if (i != except_geom_index)
		{	
			spheres[i].intersect(r, rec);
		}
	}

	for (int i = 0; i < rectangles.size(); i++)
	{
		if (i != except_geom_index)
		{
			rectangles[i].intersect(r, rec);
		}
	}
}
