#pragma once
#include "../RTEngineCPU/rt_gateway.h"
#include"../RTEngineCPU/IParser.h"
#include "json.hpp"


using namespace nlohmann;

class JsonParser : public IParser
{
private:
	vector<rt_sphere> _spheres;
	vector<rt_rectangle> _rects;
	vector<rt_material> _mats;
	vector<rt_directional_light> _dir_lights;
	vector<rt_point_light> _point_lights;
	vector<rt_spot_light> _spot_lights;
	vector <rt_area_light > _area_lights;


		void parse_helper(json& j);

		void parse_spheres(json& j_sphs);

		void parse_rects(json& j_rects);

		void parse_diffuse_materials(json& j_mats);

		void parse_lights(json& j_lights);

		void parse_directional_light(json& j_dlight);

		void parse_point_light(json& j_plight);

		void parse_spot_light(json& j_slight);

		void parse_area_light(json& j_alight);

		vector<float> json_to_vector(json& v);

		
public:
		virtual void parse(const char* input);
};