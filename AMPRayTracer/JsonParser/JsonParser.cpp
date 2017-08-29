#include "JsonParser.h"



void JsonParser::parse_helper(json& j)
{
	/*extract geometries*/

	json j_sphs = j["geometries"]["spheres"];
	parse_spheres(j_sphs);

	json j_rects = j["geometries"]["rectangles"];
	parse_rects(j_rects);

	/*extract materials*/
	json j_mats = j["materials"]["diffuse"];
	parse_diffuse_materials(j_mats);

	/*extract lights*/
	json j_lights = j["lights"];
	parse_lights(j_lights);

	
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

		vector<float> direction = json_to_vector(j_dlight["direction"]);
		if (j_dlight.find("color") != j_dlight.end())
		{
			vector<float> color = json_to_vector(j_dlight["color"]);
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

	vector<float> origin = json_to_vector(j_plight["origin"]);
	if (j_plight.find("color") != j_plight.end())
	{
		vector<float> color = json_to_vector(j_plight["color"]);
		if (j_plight.find("range") != j_plight.end() && j_plight.find("attenuation_fraction") != j_plight.end()
			&& j_plight.find("is_realistic_attuation") != j_plight.end())
		{
			float range = j_plight["range"].get<float>();
			float att_frac = j_plight["attenuation_fraction"].get<float>();
			bool is_real_att = j_plight["is_realistic_attuation"].get<bool>();
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

	vector<float> direction = json_to_vector(j_slight["direction"]);
	vector<float> origin = json_to_vector(j_slight["origin"]);
	float central_fov = j_slight["central_fov"].get<float>();
	float total_fov = j_slight["total_fov"].get<float>();
	float drop_constant = j_slight["drop_constant"].get<float>();

	if (j_slight.find("color") != j_slight.end())
	{
		vector<float> color = json_to_vector(j_slight["color"]);
		if (j_slight.find("range") != j_slight.end() && j_slight.find("attenuation_fraction") != j_slight.end()
			&& j_slight.find("is_realistic_attuation") != j_slight.end())
		{
			float range = j_slight["range"].get<float>();
			float att_frac = j_slight["attenuation_fraction"].get<float>();
			bool is_real_att = j_slight["is_realistic_attuation"].get<bool>();
			_spot_lights.push_back(rt_spot_light(origin,direction, total_fov,central_fov,drop_constant, color,range, att_frac, is_real_att));
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
	vector<float> vertices[4];
	int i = 0;
	for (auto v = j_alight["vertices"].begin(); v != j_alight["vertices"].end(); v++)
	{
		vertices[i++] = json_to_vector(*v);
	}

	vector<float> direction = json_to_vector(j_alight["direction"]);
	float padding = j_alight["padding"].get<float>();
	float drop_constant = j_alight["drop_constant"].get<float>();

	if (j_alight.find("color") != j_alight.end())
	{
		vector<float> color = json_to_vector(j_alight["color"]);
		if (j_alight.find("range") != j_alight.end() && j_alight.find("attenuation_fraction") != j_alight.end()
			&& j_alight.find("is_realistic_attuation") != j_alight.end())
		{
			float range = j_alight["range"].get<float>();
			float att_frac = j_alight["attenuation_fraction"].get<float>();
			bool is_real_att = j_alight["is_realistic_attuation"].get<bool>();
			_area_lights.push_back(rt_area_light(direction,rt_rectangle(vertices),padding, drop_constant, color, range, att_frac, is_real_att));
		}
		else
		{
			_area_lights.push_back(rt_area_light(direction, rt_rectangle(vertices), padding , drop_constant, color));
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

		vector<float> diffuse_color = json_to_vector(mat["diffuse_color"]);
		vector<float> ambient_color = json_to_vector(mat["ambient _color"]);
		if (mat.find("specular_color") != mat.end() && mat.find("specularity") != mat.end())
		{
			vector<float> specular_color = json_to_vector(mat["specular_color"]);
			float specularity = mat["specularity"].get<float>();
			_mats.push_back(rt_material(ambient_color, diffuse_color, specular_color, specularity));
		}
		else
		{
			_mats.push_back(rt_material(ambient_color, diffuse_color));
		}
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
		vector<float> center = json_to_vector(sph["center"]);
		rt_sphere s = rt_sphere(center, radius);
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
		vector<float> vertices[4];


		int i = 0;
		for (auto v_itr = j_vert.begin(); v_itr != j_vert.end(); v_itr++)
		{
			vertices[i++] = json_to_vector(*v_itr);
		}


		if (rect.find("xform") != rect.end())
		{
			//parse xform
			vector<vector<float>> x_form_mat;

			json j_xform = rect["xform"];

			for (auto x_itr = j_xform.begin(); x_itr != j_xform.end(); x_itr++)
			{
				x_form_mat.push_back(json_to_vector(*itr));
			}

			if (x_form_mat.size() != 3 || x_form_mat[0].size() != 3)
			{
				throw exception("3D transformation matrix must be 3x3!");
			}

			_rects.push_back(rt_rectangle(vertices, matrix(x_form_mat)));
		}
		else {
			_rects.push_back(rt_rectangle(vertices));
		}
	}
}

vector<float> JsonParser::json_to_vector(json& v)
{
	vector<float> result;
	for (int i =0; i < v.size(); i++)
	{
		result.push_back(v.at(i).get<float>());
	}
	return result;
}

void JsonParser::parse(const char* input)
{
	json j = json::parse(input);
	parse_helper(j);

}