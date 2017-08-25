#include "rt_gateway.h"
#include "EasyBMP.h"

using namespace rt_support::geometries;

void render_image(BMP& image, vector<float_3>& pixels, int x_res, int y_res, const char* file_name);
void render_others(BMP& image, vector<float>& pixels, int x_res, int y_res, const char* file_name);

struct res
{
	int x;
	int y;

	res(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};


int main()
{
	std::vector<accelerator> accls = accelerator::get_all();
	accls.erase(std::remove_if(accls.begin(), accls.end(), [](accelerator& a)
	{
		return a.is_emulated;
	}), accls.end());

	for (accelerator a : accls) {
		std::wcout << "  " << a.device_path << std::endl
			<< "    " << a.description << "  "<<a.dedicated_memory <<" "  << std::endl << std::endl;

	}

	accelerator::set_default(accls.front().device_path);
	std::wcout << "Found " << accls.size() << " C++ AMP hardware accelerator(s):" << std::endl;


	std::wcout << " DEFAULT:  " << accelerator::default_accelerator << endl;
	vector<res> r =  { res(64,64),res(256,144), res(320,240), res(640,360),res(640,480),res(1280,720),res(1920,1080),res(3840,2160) };

	rt_material mat1 = rt_material(float_3( 0.9f,0.9f,0.9f ),
	float_3( 0.1f,0.1f,0.1f ), float_3( 0.8f,0.8f,0.8f ), 10);

	rt_material mat2 = rt_material(float_3(0.9f,0.9f,0.3f ),
		float_3(0.2f,0.8f,0.9f ), float_3(0.4f,0.9f,0.9f ), 5);

	rt_material mat3 = rt_material(float_3(0.7f,0.3f,0.2f ),
		float_3(0.5f,0.2f,0.1f ));

	mat1.set_resource_index(0);
	mat2.set_resource_index(1);
	mat3.set_resource_index(2);
	
	vector<rt_material> materials = { mat1,mat2,mat3 };


	rt_sphere sph1 = rt_sphere(float_3(-4, 0, 2), 1.0);
	sph1.set_resource_index(0);
	rt_sphere sph2 = rt_sphere(float_3(-1, 1.5, 0), 1.5);
	sph2.set_resource_index(1);
	rt_sphere sph3 = rt_sphere(float_3(1, 1, 1), 2.0);
	sph3.set_resource_index(2);
	rt_sphere sph4 = rt_sphere(float_3(2, 3, -1), 0.5);
	sph4.set_resource_index(3);

	sph1.set_material_index(0);
	sph2.set_material_index(1);
	sph3.set_material_index(2);
	sph4.set_material_index(0);
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

	rect1.set_material_index(0);
	rect2.set_material_index(1);
	rect3.set_material_index(2);
	vector<rt_rectangle> rectangles = { rect1,rect2,rect3};
	

	//lights
	rt_directional_light d_light = rt_directional_light(float_3(8,0,0 ));
	rt_point_light p_light = rt_point_light(float_3(0,10,0 ));
	rt_spot_light s_light = rt_spot_light(float_3(0,10,0 ), float_3(0,-1,0 ), 45, 30, 2.0);
	float_3 area_vertices[] = {
		float_3(-1.0f,5.0f,1.0f ),
		float_3(1.0f,5.0f,1.0f ),
		float_3(1.0f,5.0f,-1.0f),
		float_3(-1.0f,5.0f,-1.0f )
	};
	rt_area_light a_light = rt_area_light(float_3(0,-1,0 ), rt_rectangle(area_vertices), 0.2f, 5.0);

	vector<rt_directional_light> dir_lights = { d_light };
	vector<rt_point_light> point_lights = { p_light };
	vector<rt_spot_light> spot_lights = { s_light };
	vector<rt_area_light> area_lights = { a_light };

	


	int samples = 1;
	

	scene_results results;
	for (res& rs : r)
	{
		printf("Resolution: %d x %d\n\n", rs.x, rs.y);
		rt_camera cam = rt_camera(float_3(-8, 5, -8), float_3(0, 0, 0), float_3(0, 1, 0), 45, 2);
		image_spec spec = image_spec(rs.x, rs.y, samples);
		for (int i = 0; i < 4; i++) {
			results = rt_gateway::ray_trace(spheres, rectangles,materials,dir_lights,point_lights,spot_lights,area_lights, float_3( 0.2f,0.2f,0.2f ), 0.5f, cam, spec);
		}
		printf("\n\n______________________________________\n");
	}

	BMP image;
	BMP coverage;
	BMP depth;
	
	render_image(image, results.color, r.back().x, r.back().y,"img.bmp");
	render_others(coverage, results.coverage, r.back().x, r.back().y,"cvg.bmp");
	render_others(depth, results.depth, r.back().x, r.back().y, "dpt.bmp");
	int a = 5;
	scanf_s("Enter a number: %d",a );
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