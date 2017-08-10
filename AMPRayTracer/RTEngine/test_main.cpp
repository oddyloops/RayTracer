#include "rt_gateway.h"
#include "EasyBMP.h"

using namespace rt_support::geometries;

void render_image(BMP& image, vector<float_3>& pixels, int x_res, int y_res, const char* file_name);
void render_others(BMP& image, vector<float>& pixels, int x_res, int y_res, const char* file_name);


int main()
{
	rt_sphere sph1 = rt_sphere(float_3(-4, 0, 2), 1.0);
	sph1.set_resource_index(0);
	rt_sphere sph2 = rt_sphere(float_3(-1, 1.5, 0), 1.5);
	sph2.set_resource_index(1);
	rt_sphere sph3 = rt_sphere(float_3(1, 1, 1), 2.0);
	sph3.set_resource_index(2);
	rt_sphere sph4 = rt_sphere(float_3(2, 3, -1), 0.5);
	sph4.set_resource_index(3);
	vector<rt_sphere> spheres = { sph1,sph2,sph3,sph4};

	float_3 vtx1[] = { float_3(0,1,-2), float_3(2,1,-2),float_3(2,1,2),float_3(0,1,2) };
	rt_rectangle rect1 = rt_rectangle(vtx1);
	rect1.set_resource_index(4);
	float_3 vtx2[] = { float_3(-4,0,-4), float_3(4,0,-4),float_3(4,0,4),float_3(-4,0,4) };
	rt_rectangle rect2 = rt_rectangle(vtx2);
	rect2.set_resource_index(5);
	float_3 vtx3[] = { float_3(-4,-2,4), float_3(4,-2,4),float_3(4,2,4),float_3(-4,2,4) };
	rt_rectangle rect3 = rt_rectangle(vtx3);
	rect3.set_resource_index(6);
	vector<rt_rectangle> rectangles = { rect1,rect2,rect3};

	rt_camera cam = rt_camera(float_3(-8, 5, -8), float_3(0, 0, 0), float_3(0, 1, 0), 45, 2);
	const int X_RES = 64;
	const int Y_RES = 64;
	int samples = 1;
	image_spec spec = image_spec(X_RES,Y_RES, samples);

	scene_results results;
	for (int i = 0; i < 3; i++) {
		results = rt_gateway::ray_trace(spheres, rectangles, cam, spec);
	}
	BMP image;
	BMP coverage;
	BMP depth;
	
	render_image(image, results.color, X_RES, Y_RES,"img.bmp");
	render_others(coverage, results.coverage, X_RES, Y_RES,"cvg.bmp");
	render_others(depth, results.depth, X_RES, Y_RES, "dpt.bmp");
	system("PAUSE");
}

void render_image(BMP& image, vector<float_3>& pixels, int x_res,int y_res, const char* file_name)
{
	image.SetBitDepth(24);
	image.SetSize(x_res, y_res);
	for (int x = 0; x < x_res; x++)
	{
		for (int y = 0; y < y_res; y++)
		{
			auto dest_pixel = image(x, y);
			auto src_pixel = pixels[x*y_res + y];
			dest_pixel->Red = static_cast<unsigned char>(src_pixel.r * 255);
			dest_pixel->Green = static_cast<unsigned char>(src_pixel.g * 255);
			dest_pixel->Blue = static_cast<unsigned char>(src_pixel.b * 255);
			dest_pixel->Alpha = 255;
		}
	}
	image.WriteToFile(file_name);

}

void render_others(BMP& image, vector<float>& pixels, int x_res, int y_res, const char* file_name)
{
	image.SetBitDepth(24);
	image.SetSize(x_res, y_res);
	for (int x = 0; x < x_res; x++)
	{
		for (int y = 0; y < y_res; y++)
		{
			auto dest_pixel = image(x, y);
			auto src_pixel = pixels[x*y_res + y];
			dest_pixel->Red = static_cast<unsigned char>(src_pixel * 255);
			dest_pixel->Green = static_cast<unsigned char>(src_pixel * 255);
			dest_pixel->Blue = static_cast<unsigned char>(src_pixel * 255);
			dest_pixel->Alpha = 255;
		}
	}
	image.WriteToFile(file_name);

}