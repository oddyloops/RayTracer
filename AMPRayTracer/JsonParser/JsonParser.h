#pragma once
#include "../RTEngine/rt_gateway.h"
#include"../RTEngine/IParser.h"
#include "json.hpp"
#include <tuple>

using namespace nlohmann;

class JsonParser : public IParser
{
private:


	vector<rt_sphere> _spheres;
	vector<rt_rectangle> _rects;
	vector<rt_triangle> _triangles;
	vector<rt_plane> _planes;
	vector<rt_cylinder> _cylinders;
	vector<rt_material> _mats;
	vector<rt_directional_light> _dir_lights;
	vector<rt_point_light> _point_lights;
	vector<rt_spot_light> _spot_lights;
	vector <rt_area_light > _area_lights;
	map<int, tuple<string,int,int>> _vec_bmps; //a mapping of bitmap filename to its dimensions
	map<int, tuple<string,int, int>> _flt_bmps;
	float _max_bmp_width, _max_bmp_height;  //maximum dimension of a bitmap
	vector<float_3> _vec_scalars;
	vector<float> _flt_scalars;

	rt_camera _camera;
	image_spec _specs;
	float_3 _ambient_color;
	float _ambient_intensity;


	void parse_helper(json& j);

	void parse_max_bmp_dim(json& j_dim);

	void parse_bmps(json& j_vec_bmps,bool is_vector);

	void parse_vector_scalars(json& j_vec_scalars);

	void parse_float_scalars(json& j_flt_scalars);

	void parse_spheres(json& j_sphs);

	void parse_rects(json& j_rects);

	void parse_triangles(json& j_triangles);

	void parse_planes(json& j_planes);

	void parse_cylinders(json& j_cylinders);

	void parse_diffuse_materials(json& j_mats);

	void parse_lights(json& j_lights);

	void parse_directional_light(json& j_dlight);

	void parse_point_light(json& j_plight);

	void parse_spot_light(json& j_slight);

	void parse_area_light(json& j_alight);

	void parse_camera(json& j_cam);

	void parse_image_spec(json& j_spec);

	float_3 json_to_vector(json& v);

	float_2 json_to_vector_2d(json& v);

	template<typename T>
	texture_map<T> json_to_map(json& j);

	void render();

public:
	virtual void parse(const char* input);
};


template<typename T>
texture_map<T> JsonParser::json_to_map(json& j)
{
	if (j.find("is_bitmap") == j.end())
	{
		throw exception("invalid mapping structure");
	}

	bool is_bitmap = j["is_bitmap"].get<bool>();
	if (is_bitmap)
	{
		if (j.find("hor_offset") == j.end() || j.find("ver_offset") == j.end()
			|| j.find("ver_unit_length") == j.end() || j.find("ver_unit_length") == j.end()
			|| j.find("index") == j.end())
		{
			throw exception("invalid bitmap mapping");
		}
		int index = j["index"].get<int>();
		float hor_offset = j["hor_offset"].get<float>();
		float ver_offset = j["ver_offset"].get<float>();
		float hor_unit_length = j["hor_unit_length"].get<float>();
		float ver_unit_length = j["ver_unit_length"].get<float>();

		if ((hor_offset + hor_unit_length - 1) > FLT_EPSILON || (ver_offset + ver_unit_length - 1) > FLT_EPSILON)
		{
			throw exception("Mapping out of bounds");
		}
		int width = (typeid(T) == typeid(float_3)) ? get<1>(_vec_bmps[index] ): get<1>(_flt_bmps[index]);
		int height = (typeid(T) == typeid(float_3)) ? get<2>(_vec_bmps[index]) : get<2>(_flt_bmps[index]);

		int i_hor_offset = static_cast<int>(hor_offset * width);
		int i_ver_offset = static_cast<int>(ver_offset * height);
		int i_width = static_cast<int>(hor_unit_length * width);
		int i_height = static_cast<int>(ver_unit_length * height);
		texture_map<T> map = texture_map<T>(true, map_type::plain);
		map.set_bitmap_index(index, i_hor_offset, i_ver_offset, i_width, i_height);
		return map;
	}
	else
	{
		if (j.find("mapping") == j.end() || j.find("color_offset") == j.end()
			|| j.find("colors_length") == j.end())
		{
			throw exception("Invalid scalar mapping");
		}

		string mapping = j["mapping"].get<string>();

		texture_map<T> map;
		if (mapping == "checkered")
		{
			if (j.find("unit_check_height") == j.end() || j.find("unit_check_width") == j.end())
			{
				throw exception("Invalid checkered mapping structure");
			}

			float check_width = j["unit_check_width"].get<float>();
			float check_height = j["unit_check_height"].get<float>();
			map = texture_map<T>(false, map_type::checkered);
			map.set_checker_dim(check_width, check_height);

		}
		else if (mapping == "stripes")
		{
			if (j.find("direction") == j.end() || j.find("stripe_unit_width") == j.end())
			{
				throw exception("Invalid stripes mapping structure");
			}
			map = texture_map<T>(false, map_type::stripes);
			float_2 direction = json_to_vector_2d(j["direction"]);
			float stripe_unit_width = j["stripe_unit_width"].get<float>();
			map.set_stripe_width_perc(stripe_unit_width);
			map.set_direction(direction);

		}
		else if (mapping == "wavy")
		{
			map = texture_map<T>(false, map_type::wavy);
			if (j.find("direction") == j.end() || j.find("stripe_unit_width") == j.end())
			{
				throw exception("Invalid stripes mapping structure");
			}
			map = texture_map<T>(false, map_type::wavy);
			float_2 direction = json_to_vector_2d(j["direction"]);
			float stripe_unit_width = j["stripe_unit_width"].get<float>();
			map.set_stripe_width_perc(stripe_unit_width);
			map.set_direction(direction);
		}
		else
		{
			map = texture_map<T>(false, map_type::plain);
		}
		map.set_scalar_index(j["color_offset"].get<int>(), j["colors_length"].get<int>());
		return map;
	}
}



