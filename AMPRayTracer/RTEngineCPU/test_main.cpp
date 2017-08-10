
#include "rt_gateway.h"
#include "EasyBMP.h"
#include "vector_util.h"
#include <string>

using namespace rt_support::geometries;

int main()
{
	
	rt_sphere sph1 = rt_sphere({ -4, 0, 2 }, 1);
	rt_sphere sph2 = rt_sphere({ -1, 1.5, 0 }, 1.5);
	rt_sphere sph3 = rt_sphere({ 1, 1, 1 }, 2);
	rt_sphere sph4 = rt_sphere({ 2, 3, -1 }, 0.5);
	sph1.set_resource_index(0);
	sph2.set_resource_index(1);
	sph3.set_resource_index(2);
	sph4.set_resource_index(3);
	vector<rt_sphere> spheres = {sph1,sph2,sph3,sph4};

	vector<float> vtx1[] = { { 0,1,-2 }, { 2,1,-2 },{ 2,1,2 },{ 0,1,2 } };
	rt_rectangle rect1 = rt_rectangle(vtx1);
	vector<float> vtx2[] = { {-4,0,-4}, {4,0,-4},{4,0,4},{-4,0,4} };
	rt_rectangle rect2 = rt_rectangle(vtx2);
	vector<float> vtx3[] = { {-4,-2,4}, {4,-2,4},{4,2,4},{-4,2,4} };
	rt_rectangle rect3 = rt_rectangle(vtx3);
	rect1.set_resource_index(4);
	rect2.set_resource_index(5);
	rect3.set_resource_index(6);
	vector<rt_rectangle> rectangles = { rect1,rect2,rect3 };

	const int X_RES = 1920;
	const int Y_RES = 1080;

	rt_camera cam = rt_camera({ -8, 5, -8 }, { 0, 0, 0 }, { 0, 1, 0 }, 45, 2);
	image_spec spec = image_spec(X_RES, Y_RES, 1);
	
	scene_results results;
	for (int i = 0; i < 3; i++) {
		results = rt_gateway::ray_trace(spheres, rectangles, cam, spec);
	}
	

	int img_count = 1;
	for (auto& image : results)
	{
		BMP input;
		input.SetSize(X_RES, Y_RES);
		for (int x = 0; x < X_RES; x++)
		{
			for (int y = 0; y < Y_RES; y++)
			{
				int index = x * Y_RES + y;
				if (image[0].size() > 1)
				{
					input(x, y)->Alpha = 255;
					
					
					
					input(x, y)->Red = static_cast<unsigned char>(static_cast<int>(image[index][0] * 255));
					input(x, y)->Green = static_cast<unsigned char>(static_cast<int>(image[index][1] * 255));
					input(x, y)->Blue = static_cast<unsigned char>(static_cast<int>(image[index][2] * 255));
				}
				else
				{
					input(x, y)->Alpha = 255;
					input(x, y)->Red = static_cast<unsigned char>(static_cast<int>(image[index][0] * 255));
					input(x, y)->Green = static_cast<unsigned char>(static_cast<int>(image[index][0] * 255));
					input(x, y)->Blue = static_cast<unsigned char>(static_cast<int>(image[index][0] * 255));
				}

			}
		}
		input.WriteToFile((string("img") + to_string(img_count++) + string(".bmp")).c_str());
	}

	system("PAUSE");
}