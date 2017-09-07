#pragma once

#include<vector>


#include "rt_visibility.h"
#include "ray.h"
#include "intersection_record.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "pixel_data.h"
#include "scene_database.h"


using namespace std;


using namespace rt_support;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
using namespace rt_support::scene_resource_support;


namespace rt
{
	class rt_shader
	{
		scene_database* m_db;
		vector<float> m_ambient_light;
		float m_ambient_intensity;
		vector<float> m_view_dir;
		rt_visibility m_visible; //used for reflection and refraction computations

		vector<float> compute_ambience(rt_material& mat);

		vector<float> compute_diffuse(intersection_record& rec, rt_material& mat,  int generation);

		vector<float> compute_specular(intersection_record& rec, rt_material& mat, int generation);

		rt_material get_material_from_index(int mat_res_index);

		vector<float> color_bounce_lights(vector<float>& light_dir, intersection_record& prev_rec, int generation);

		void reflect_and_refract(intersection_record current_rec, vector<float> light_dir, float percent_light, rt_material& mat, vector<float>& result_color
			, int generation);
	public:
		rt_shader();

		rt_shader(scene_database* db,vector<float> ambient_light,float ambient_intensity,vector<float> view_dir);

		vector<float> compute_shade(intersection_record& rec, int generation);

		
	};
}