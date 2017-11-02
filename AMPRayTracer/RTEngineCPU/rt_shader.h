#pragma once

#include<vector>



#include "ray.h"
#include "intersection_record.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "pixel_data.h"
#include "scene_database.h"
#include "rt_visibility.h"


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
		rt_visibility m_visibility; //used for reflected and refracted ray intersection
		vector<float> m_ambient_light;
		float m_ambient_intensity;
		vector<float> m_view_dir;

		vector<float> compute_ambience(rt_material& mat,intersection_record& rec);

		vector<float> compute_diffuse(intersection_record& rec, rt_material& mat);

		vector<float> compute_specular(intersection_record& rec, rt_material& mat);
	public:
		rt_shader();

		rt_shader(scene_database* db, vector<float> ambient_light, float ambient_intensity, vector<float> view_dir);

		vector<float> compute_shade(intersection_record& rec, int generation);


	};
}