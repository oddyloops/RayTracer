#include "JsonParser.h"
#include "EasyBMP.h"


void JsonParser::parse_helper(json& j)
{
	/*extract geometries*/

	json j_sphs = j["geometries"]["spheres"];
	parse_spheres(j_sphs);

	json j_rects = j["geometries"]["rectangles"];
	parse_rects(j_rects);

	json j_triangles = j["geometries"]["triangles"];
	parse_triangles(j_triangles);

	json j_planes = j["geometries"]["planes"];
	parse_planes(j_planes);

	json j_cylinders = j["geometries"]["cylinders"];
	parse_cylinders(j_cylinders);

	/*extract materials*/
	json j_mats = j["materials"]["solids"];
	parse_diffuse_materials(j_mats);

	/*extract lights*/
	json j_lights = j["lights"];
	parse_lights(j_lights);

	/*extract camera*/
	json j_cam = j["camera"];
	parse_camera(j_cam);

	/*extract image specs*/
	json j_spec = j["specs"];
	parse_image_spec(j_spec);

	/*extract ambient properties*/
	_ambient_color = json_to_vector(j["ambient_color"]);
	_ambient_intensity = j["ambient_intensity"].get<float>();


}


void JsonParser::parse_lights(json& j_lights)
{

	for (auto itr = j_lights.begin(); itr != j_lights.end(); itr++)
	{
		json light = *itr;
		if (light.find("type") == light.end())
		{
			throw exception("Invalid light structure");
		}

		if (light["type"].get<string>() == "directional")
		{
			parse_directional_light(light);
		}
		else if (light["type"].get<string>() == "point")
		{
			parse_point_light(light);
		}
		else if (light["type"].get<string>() == "spot")
		{
			parse_spot_light(light);
		}
		else if (light["type"].get<string>() == "area")
		{
			parse_area_light(light);
		}
		else
		{
			throw exception("Invalid light type");
		}

	}
}

void JsonParser::parse_directional_light(json& j_dlight)
{
	if (j_dlight.find("direction") == j_dlight.end() || j_dlight.find("origin") != j_dlight.end()
		|| j_dlight.find("range") != j_dlight.end() || j_dlight.find("attenuation_factor") != j_dlight.end()
		|| j_dlight.find("is_realistic_attuation") != j_dlight.end())
	{
		throw exception("Incorrect directional light structure");
	}

	float_3 direction = json_to_vector(j_dlight["direction"]);
	if (j_dlight.find("color") != j_dlight.end())
	{
		float_3 color = json_to_vector(j_dlight["color"]);
		_dir_lights.push_back(rt_directional_light(direction, color));
	}
	else
	{
		_dir_lights.push_back(rt_directional_light(direction));
	}

}

void JsonParser::parse_point_light(json& j_plight)
{
	if (j_plight.find("direction") != j_plight.end() || j_plight.find("origin") == j_plight.end())
	{
		throw exception("Incorrect point light structure");
	}

	float_3 origin = json_to_vector(j_plight["origin"]);
	if (j_plight.find("color") != j_plight.end())
	{
		float_3 color = json_to_vector(j_plight["color"]);
		if (j_plight.find("range") != j_plight.end() && j_plight.find("attenuation_fraction") != j_plight.end()
			&& j_plight.find("is_realistic_attuation") != j_plight.end())
		{
			float range = j_plight["range"].get<float>();
			float att_frac = j_plight["attenuation_fraction"].get<float>();
			int is_real_att = j_plight["is_realistic_attuation"].get<bool>() ? 1 : 0;
			_point_lights.push_back(rt_point_light(origin, color, range, att_frac, is_real_att));
		}
		else
		{
			_point_lights.push_back(rt_point_light(origin, color));
		}

	}
	else
	{
		_point_lights.push_back(rt_point_light(origin));
	}
}

void JsonParser::parse_spot_light(json& j_slight)
{
	if (j_slight.find("direction") == j_slight.end() || j_slight.find("origin") == j_slight.end()
		|| j_slight.find("central_fov") == j_slight.end() || j_slight.find("total_fov") == j_slight.end()
		|| j_slight.find("drop_constant") == j_slight.end())
	{
		throw exception("Incorrect spot light structure");
	}

	float_3 direction = json_to_vector(j_slight["direction"]);
	float_3 origin = json_to_vector(j_slight["origin"]);
	float central_fov = j_slight["central_fov"].get<float>();
	float total_fov = j_slight["total_fov"].get<float>();
	float drop_constant = j_slight["drop_constant"].get<float>();

	if (j_slight.find("color") != j_slight.end())
	{
		float_3 color = json_to_vector(j_slight["color"]);
		if (j_slight.find("range") != j_slight.end() && j_slight.find("attenuation_fraction") != j_slight.end()
			&& j_slight.find("is_realistic_attuation") != j_slight.end())
		{
			float range = j_slight["range"].get<float>();
			float att_frac = j_slight["attenuation_fraction"].get<float>();
			int is_real_att = j_slight["is_realistic_attuation"].get<bool>() ? 1 : 0;
			_spot_lights.push_back(rt_spot_light(origin, direction, total_fov, central_fov, drop_constant, color, range, att_frac, is_real_att));
		}
		else
		{
			_spot_lights.push_back(rt_spot_light(origin, direction, total_fov, central_fov, drop_constant, color));
		}

	}
	else
	{
		_spot_lights.push_back(rt_spot_light(origin, direction, total_fov, central_fov, drop_constant));
	}
}

void JsonParser::parse_area_light(json& j_alight)
{
	if (j_alight.find("direction") == j_alight.end() || j_alight.find("origin") != j_alight.end()
		|| j_alight.find("vertices") == j_alight.end() || j_alight.find("padding") == j_alight.end()
		|| j_alight.find("drop_constant") == j_alight.end())
	{
		throw exception("Incorrect area light structure");
	}

	if (j_alight["vertices"].size() != 4)
	{
		throw exception("Area light must have exactly 4 vertices");
	}
	float_3 vertices[4];
	int i = 0;
	for (auto v = j_alight["vertices"].begin(); v != j_alight["vertices"].end(); v++)
	{
		vertices[i++] = json_to_vector(*v);
	}

	float_3 direction = json_to_vector(j_alight["direction"]);
	float padding = j_alight["padding"].get<float>();
	float drop_constant = j_alight["drop_constant"].get<float>();

	if (j_alight.find("color") != j_alight.end())
	{
		float_3 color = json_to_vector(j_alight["color"]);
		if (j_alight.find("range") != j_alight.end() && j_alight.find("attenuation_fraction") != j_alight.end()
			&& j_alight.find("is_realistic_attuation") != j_alight.end())
		{
			float range = j_alight["range"].get<float>();
			float att_frac = j_alight["attenuation_fraction"].get<float>();
			int is_real_att = j_alight["is_realistic_attuation"].get<bool>() ? 1 : 0;
			_area_lights.push_back(rt_area_light(direction, rt_rectangle(vertices), padding, drop_constant, color, range, att_frac, is_real_att));
		}
		else
		{
			_area_lights.push_back(rt_area_light(direction, rt_rectangle(vertices), padding, drop_constant, color));
		}

	}
	else
	{
		_area_lights.push_back(rt_area_light(direction, rt_rectangle(vertices), padding, drop_constant));
	}
}

void JsonParser::parse_diffuse_materials(json& j_mats)
{
	for (auto itr = j_mats.begin(); itr != j_mats.end(); itr++)
	{
		json mat = *itr;
		if (mat.find("diffuse_color") == mat.end() || mat.find("ambient_color") == mat.end() ||
			mat.find("resource_index") == mat.end())
		{
			throw exception("Incorrect diffuse material structure");
		}

		float_3 diffuse_color = json_to_vector(mat["diffuse_color"]);
		float_3 ambient_color = json_to_vector(mat["ambient_color"]);
		rt_material m;
		if (mat.find("specular_color") != mat.end() && mat.find("specularity") != mat.end())
		{
			float_3 specular_color = json_to_vector(mat["specular_color"]);
			float specularity = mat["specularity"].get<float>();
			m = rt_material(ambient_color, diffuse_color, specular_color, specularity);

		}
		else
		{
			m = rt_material(ambient_color, diffuse_color);
		}
		m.set_ref_properties(mat["refractive_index"].get<float>(), mat["transparency"].get<float>(), mat["reflectivity"].get<float>());
		m.set_resource_index(mat["resource_index"].get<int>());
		_mats.push_back(m);
	}
}


void JsonParser::parse_spheres(json& j_sphs)
{
	for (auto itr = j_sphs.begin(); itr != j_sphs.end(); itr++)
	{

		json sph = *itr;
		if (sph.find("radius") == sph.end() || sph.find("center") == sph.end() ||
			sph.find("resource_index") == sph.end())
		{
			throw exception("Incorrect sphere structure");
		}
		float radius = sph["radius"].get<float>();
		float_3 center = json_to_vector(sph["center"]);
		rt_sphere s = rt_sphere(center, radius);
		s.set_resource_index(sph["resource_index"].get<int>());
		if (sph.find("material_index") != sph.end())
		{
			s.set_material_index(sph["material_index"].get<int>());
		}
		_spheres.push_back(s);
	}
}

void JsonParser::parse_rects(json& j_rects)
{

	for (auto itr = j_rects.begin(); itr != j_rects.end(); itr++)
	{
		json rect = *itr;
		if (rect.find("vertices") == rect.end() || rect.find("resource_index") == rect.end())
		{
			throw exception("Incorrect rectangle structure");
		}

		json j_vert = rect["vertices"];
		if (j_vert.size() != 4)
		{
			throw exception("Rectangles must have exactly 4 vertices!");
		}
		float_3 vertices[4];


		int i = 0;
		for (auto v_itr = j_vert.begin(); v_itr != j_vert.end(); v_itr++)
		{
			vertices[i++] = json_to_vector(*v_itr);
		}

		rt_rectangle r;
		if (rect.find("xform") != rect.end())
		{
			//parse xform
			concurrency::array<float, 2> x_form_mat(3, 3);

			json j_xform = rect["xform"];
			int rowIndex = 0;
			for (auto x_itr = j_xform.begin(); x_itr != j_xform.end(); x_itr++, rowIndex++)
			{
				float_3 row = json_to_vector(*itr);
				x_form_mat[index<2>(rowIndex, 0)] = row.x;
				x_form_mat[index<2>(rowIndex, 1)] = row.y;
				x_form_mat[index<2>(rowIndex, 2)] = row.z;
			}

			if (rowIndex != 3)
			{
				throw exception("3D transformation matrix must be 3x3!");
			}
			r = rt_rectangle(vertices, x_form_mat);

		}
		else {
			r = rt_rectangle(vertices);
		}
		r.set_resource_index(rect["resource_index"].get<int>());
		if (rect.find("material_index") != rect.end())
		{
			r.set_material_index(rect["material_index"].get<int>());
		}
		_rects.push_back(r);
	}
}

void JsonParser::parse_triangles(json& j_triangles)
{
	for (auto itr = j_triangles.begin(); itr != j_triangles.end(); itr++)
	{
		json tri = *itr;

		if (tri.find("vertices") == tri.end() || tri.find("resource_index") == tri.end())
		{
			throw exception("Incorrect triangle structure");
		}

		json j_vert = tri["vertices"];
		if (j_vert.size() != 3)
		{
			throw exception("Triangles must have exactly 3 vertices!");
		}
		float_3 vertices[3];

		int i = 0;
		for (auto v_itr = j_vert.begin(); v_itr != j_vert.end(); v_itr++)
		{
			vertices[i++] = json_to_vector(*v_itr);
		}

		rt_triangle t;
		if (tri.find("xform") != tri.end())
		{
			//parse xform
			concurrency::array<float, 2> x_form_mat(3, 3);

			json j_xform = tri["xform"];
			int rowIndex = 0;
			for (auto x_itr = j_xform.begin(); x_itr != j_xform.end(); x_itr++, rowIndex++)
			{
				float_3 row = json_to_vector(*itr);
				x_form_mat[index<2>(rowIndex, 0)] = row.x;
				x_form_mat[index<2>(rowIndex, 1)] = row.y;
				x_form_mat[index<2>(rowIndex, 2)] = row.z;
			}

			if (rowIndex != 3)
			{
				throw exception("3D transformation matrix must be 3x3!");
			}
			t = rt_triangle(vertices, x_form_mat);

		}
		else {
			t = rt_triangle(vertices);
		}
		t.set_resource_index(tri["resource_index"].get<int>());
		if (tri.find("material_index") != tri.end())
		{
			t.set_material_index(tri["material_index"].get<int>());
		}
		_triangles.push_back(t);
	}
}
void JsonParser::parse_planes(json& j_planes)
{
	for (auto itr = j_planes.begin(); itr != j_planes.end(); itr++)
	{
		json plane = *itr;
		if (plane.find("points") == plane.end() || plane.find("resource_index") == plane.end())
		{
			throw exception("Incorrect plane structure");
		}
		json points = plane["points"];
		if (points.size() != 3)
		{
			throw exception("Plane needs to be defined by 3 points");
		}
		float_3 points_vector[3];
		int i = 0;
		for (auto p_itr = points.begin(); p_itr != points.end(); p_itr++)
		{
			points_vector[i++] = json_to_vector(*p_itr);
		}

		rt_plane p = rt_plane(points_vector);
		p.set_resource_index(plane["resource_index"].get<int>());
		if (plane.find("material_index") != plane.end())
		{
			p.set_material_index(plane["material_index"].get<int>());
		}
		_planes.push_back(p);

	}
}

void JsonParser::parse_cylinders(json& j_cylinders)
{
	for (auto itr = j_cylinders.begin(); itr != j_cylinders.end(); itr++)
	{
		json cylinder = *itr;

		if (cylinder.find("radius") == cylinder.end() || cylinder.find("top_center") == cylinder.end()
			|| cylinder.find("bottom_center") == cylinder.end() || cylinder.find("resource_index") == cylinder.end())
		{
			throw exception("Incorrect cylinder structure");
		}

		float radius = cylinder["radius"].get<float>();
		float_3 top_center = json_to_vector(cylinder["top_center"]);
		float_3 bottom_center = json_to_vector(cylinder["bottom_center"]);

		rt_cylinder c = rt_cylinder(radius, top_center, bottom_center);
		c.set_resource_index(cylinder["resource_index"].get<int>());

		if (cylinder.find("material_index") != cylinder.end())
		{
			c.set_material_index(cylinder["material_index"].get<int>());
		}
		_cylinders.push_back(c);
	}
}


void JsonParser::parse_camera(json& j_cam)
{
	if (j_cam.find("eye") == j_cam.end() || j_cam.find("is_orthographic") == j_cam.end() ||
		j_cam.find("at") == j_cam.end() || j_cam.find("focus") == j_cam.end() ||
		j_cam.find("fov") == j_cam.end() || j_cam.find("up") == j_cam.end())
	{
		throw exception("Incorrect camera structure!");
	}

	float_3 eye = json_to_vector(j_cam["eye"]);
	float_3 at = json_to_vector(j_cam["at"]);
	float_3 up = json_to_vector(j_cam["up"]);
	int is_ortho = j_cam["is_orthographic"].get<bool>() ? 1 : 0;
	float focus = j_cam["focus"].get<float>();
	float fov = j_cam["fov"].get<float>();

	_camera = rt_camera(eye, at, up, fov, focus);
	_camera.set_ortho_mode_on(is_ortho);
	_camera.set_generation(j_cam["generation"].get<int>());


}


void JsonParser::parse_image_spec(json& j_spec)
{
	if (j_spec.find("x_res") == j_spec.end() || j_spec.find("y_res") == j_spec.end() ||
		j_spec.find("samples_per_pixel") == j_spec.end())
	{
		throw exception("Incorrect image spec structure!");
	}
	int x_res = j_spec["x_res"].get<int>();
	int y_res = j_spec["y_res"].get<int>();
	int samples = j_spec["samples_per_pixel"].get<int>();

	_specs = image_spec(x_res, y_res, samples);
}

float_3 JsonParser::json_to_vector(json& v)
{
	float_3 result;
	result.x = v.at(0).get<float>();
	result.y = v.at(1).get<float>();
	result.z = v.at(2).get<float>();

	return result;
}

void JsonParser::parse(const char* input)
{
	json j = json::parse(input);
	parse_helper(j);
	render();

}

void JsonParser::render()
{
	auto results = rt_gateway::ray_trace(_spheres, _rects,_triangles,_planes,_cylinders, _mats, _dir_lights, _point_lights, _spot_lights, _area_lights, _ambient_color, _ambient_intensity,
		_camera, _specs);

	auto image = results.color;

	BMP input;
	input.SetSize(_specs.get_x_resolution(), _specs.get_y_resolution());
	for (int x = 0; x < _specs.get_x_resolution(); x++)
	{
		for (int y = 0; y < _specs.get_y_resolution(); y++)
		{
			int index = x * _specs.get_y_resolution() + y;
			input(x, y)->Alpha = 255;
			input(x, y)->Red = static_cast<unsigned char>(static_cast<int>(image[index].r * 255));
			input(x, y)->Green = static_cast<unsigned char>(static_cast<int>(image[index].g * 255));
			input(x, y)->Blue = static_cast<unsigned char>(static_cast<int>(image[index].b * 255));

		}
	}
	input.WriteToFile("img.bmp");


	auto cvg = results.coverage;


	input.SetSize(_specs.get_x_resolution(), _specs.get_y_resolution());
	for (int x = 0; x < _specs.get_x_resolution(); x++)
	{
		for (int y = 0; y < _specs.get_y_resolution(); y++)
		{
			int index = x * _specs.get_y_resolution() + y;
			input(x, y)->Alpha = 255;
			input(x, y)->Red = static_cast<unsigned char>(static_cast<int>(cvg[index] * 255));
			input(x, y)->Green = static_cast<unsigned char>(static_cast<int>(cvg[index] * 255));
			input(x, y)->Blue = static_cast<unsigned char>(static_cast<int>(cvg[index] * 255));

		}
	}
	input.WriteToFile("cvg.bmp");

	auto dpt = results.depth;

	input.SetSize(_specs.get_x_resolution(), _specs.get_y_resolution());
	for (int x = 0; x < _specs.get_x_resolution(); x++)
	{
		for (int y = 0; y < _specs.get_y_resolution(); y++)
		{
			int index = x * _specs.get_y_resolution() + y;
			input(x, y)->Alpha = 255;
			input(x, y)->Red = static_cast<unsigned char>(static_cast<int>(dpt[index] * 255));
			input(x, y)->Green = static_cast<unsigned char>(static_cast<int>(dpt[index] * 255));
			input(x, y)->Blue = static_cast<unsigned char>(static_cast<int>(dpt[index] * 255));

		}
	}
	input.WriteToFile("dpt.bmp");


	system("PAUSE");

}