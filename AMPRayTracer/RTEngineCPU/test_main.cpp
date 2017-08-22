
#include "rt_gateway.h"
#include "EasyBMP.h"
#include "vector_util.h"
#include <string>

using namespace rt_support::geometries;

struct res
{
	int x;
	int y;

	res(int x, int y)
	{
		this->x = x;
		this-> y = y;
	}
};

int main()
{
	vector<res> r = { res(1920,1080) };
		// { res(64,64),res(256,144), res(320,240), res(640,360),res(640,480),res(1280,720),res(1920,1080)};
	//materials
	rt_material mat1 = rt_material({ 0.9f,0.9f,0.9f },
	{ 0.5f,0.1f,0.3f }, { 0.9f,0.2f,0.6f }, 10);

	rt_material mat2 = rt_material({ 0.9f,0.9f,0.3f },
	{ 0.2f,0.8f,0.9f }, { 0.9f,0.9f,0.9f }, 2);

	rt_material mat3 = rt_material({ 0.7f,0.3f,0.2f },
	{ 0.5f,0.2f,0.1f });

	mat1.set_resource_index(0);
	mat2.set_resource_index(1);
	mat3.set_resource_index(2);

	vector<rt_material> materials = { mat1,mat2,mat3 };
	//spheres
	rt_sphere sph1 = rt_sphere({ -4, 0, 2 }, 1);
	rt_sphere sph2 = rt_sphere({ -1, 1.5, 0 }, 1.5);
	rt_sphere sph3 = rt_sphere({ 1, 1, 1 }, 2);
	rt_sphere sph4 = rt_sphere({ 2, 3, -1 }, 0.5);
	sph1.set_resource_index(0);
	sph2.set_resource_index(1);
	sph3.set_resource_index(2);
	sph4.set_resource_index(3);
	sph1.set_material_index(0);
	sph2.set_material_index(1);
	sph3.set_material_index(2);
	sph4.set_material_index(0);
	vector<rt_sphere> spheres = {sph1,sph2,sph3,sph4};
	//rects
	vector<float> vtx1[] = { { 0,1,-2 }, { 2,1,-2 },{ 2,1,2 },{ 0,1,2 } };
	rt_rectangle rect1 = rt_rectangle(vtx1);
	vector<float> vtx2[] = { {-4,0,-4}, {4,0,-4},{4,0,4},{-4,0,4} };
	rt_rectangle rect2 = rt_rectangle(vtx2);
	vector<float> vtx3[] = { {-4,-2,4}, {4,-2,4},{4,2,4},{-4,2,4} };
	rt_rectangle rect3 = rt_rectangle(vtx3);
	rect1.set_resource_index(4);
	rect2.set_resource_index(5);
	rect3.set_resource_index(6);
	rect1.set_material_index(0);
	rect2.set_material_index(1);
	rect3.set_material_index(2);
	vector<rt_rectangle> rectangles = { rect1,rect2,rect3 };
	
	//lights
	rt_directional_light d_light = rt_directional_light({ 8,-5,0 });
	rt_point_light p_light = rt_point_light({ 0,10,0 });
	rt_spot_light s_light = rt_spot_light({ 0,10,0 }, { 0,-1,0 }, 45, 30, 2.0);
	vector<float> area_vertices[] = {
		{ -1.0f,5.0f,1.0f },
		{ 1.0f,5.0f,1.0f },
		{ 1.0f,5.0f,-1.0f },
		{ -1.0f,5.0f,-1.0f }
	};
	rt_area_light a_light = rt_area_light( { 0,-1,0 }, rt_rectangle(area_vertices), 0.2f, 5.0);

	vector<rt_directional_light> dir_lights = { d_light };
	vector<rt_point_light> point_lights = { p_light };
	vector<rt_spot_light> spot_lights = { s_light };
	vector<rt_area_light> area_lights = { a_light };

	const int X_RES = 256;
	const int Y_RES = 144;

	
	
	scene_results results;

	for (res& rs : r)
	{
		printf("Resolution: %d x %d\n\n", rs.x, rs.y);
		rt_camera cam = rt_camera({ -8, 5, -8 }, { 0, 0, 0 }, { 0, 1, 0 }, 45, 2);
		image_spec spec = image_spec(rs.x, rs.y, 1);
		for (int i = 0; i < 1; i++) {
			results = rt_gateway::ray_trace(spheres, rectangles, materials, dir_lights, point_lights, spot_lights, area_lights, { 0.2f,0.2f,0.2f }, 0.5f, cam, spec);
		}
		printf("\n\n______________________________________\n");
	}

	int img_count = 1;
	for (auto& image : results)
	{
		BMP input;
		input.SetSize(r.back().x, r.back().y);
		for (int x = 0; x < r.back().x; x++)
		{
			for (int y = 0; y < r.back().y; y++)
			{
				int index = x * r.back().y + y;
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