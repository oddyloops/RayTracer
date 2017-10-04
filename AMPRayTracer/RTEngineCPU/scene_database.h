#pragma once

#include <vector>
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "rt_triangle.h"
#include "rt_plane.h"
#include "rt_material.h"
#include "rt_area_light.h"
#include "rt_directional_light.h"
#include "rt_spot_light.h"
#include "rt_point_light.h"

using namespace std;
using namespace rt_support::geometries;
using namespace rt_support::lights;
using namespace rt_support::scene_resource_support;

namespace rt_support
{
	/// <summary>
	/// Scene data base for the RT engine, including:
	/// 
	///     1. Geometries: 
	///     2. Materials
	///     3. Textures
	///     4. Light sources
	///     
	/// </summary>

	/*
	For:
	Geometry and
	Light,

	you can loop through them as an array:

	SceneDatabase myDatabase;
	...
	// geometry ...
	for (int i = 0; i < myDatabase.GetNumGeom(); i++) {
	RTGeometry g = mSceneDatabase.GetGeom(i);
	g.Dosomething ...
	}
	...
	// light ...
	for (int i = 0; i < myDatabase.GetNumLights(); i++) {
	RTLight l = mSceneDatabase.GetLight(i);
	l.Dosomething ...
	}

	For:
	Texture and Materials

	you should only access the resource when you know their index!!
	*/
	class scene_database
	{
	private:
		vector<rt_sphere> m_spheres;
		vector<rt_rectangle> m_rects;
		vector<rt_triangle> m_triangles;
		vector<rt_plane> m_planes;
		vector<rt_material> m_materials;
		vector<rt_area_light> m_area_lights;
		vector<rt_spot_light> m_spot_lights;
		vector<rt_directional_light> m_directional_lights;
		vector<rt_point_light> m_point_lights;
		
	

	public:
		scene_database();
		scene_database(vector<rt_sphere> _spheres, vector<rt_rectangle> _rects,vector<rt_triangle> _triangles,vector<rt_plane> _plane,vector<rt_material> materials,
			vector<rt_area_light> area_lights, vector<rt_spot_light> spot_lights, vector<rt_directional_light> directional_lights,
			vector<rt_point_light> point_lights);

		void add_sphere(rt_sphere s);
		void add_rect(rt_rectangle r);
		void add_triangle(rt_triangle t);
		void add_plane(rt_plane p);
		void add_material(rt_material m);
		void add_area_light(rt_area_light a);
		void add_directional_light(rt_directional_light d);
		void add_spot_light(rt_spot_light s);
		void add_point_light(rt_point_light p);

		rt_sphere get_sphere(int index);
		rt_rectangle get_rectangle(int index);
		rt_triangle get_triangle(int index);
		rt_plane get_plane(int index);
		rt_material get_material(int index);
		rt_area_light get_area_light(int index);
		rt_directional_light get_directional_light(int index);
		rt_spot_light get_spot_light(int index);
		rt_point_light get_point_light(int index);
		
		vector<rt_sphere>& get_all_spheres();
		vector<rt_rectangle>& get_all_rectangles();
		vector<rt_triangle>& get_all_triangles();
		vector<rt_plane>& get_all_planes();
		vector<rt_material>& get_all_materials();
		vector<rt_directional_light>& get_all_directional_lights();
		vector<rt_area_light>& get_all_area_lights();
		vector<rt_spot_light>& get_spot_lights();
		vector<rt_point_light>& get_point_lights();


		int get_num_spheres();
		int get_num_rects();
		int get_num_triangles();
		int get_num_planes();
		int get_num_materials();
		int get_num_area_lights();
		int get_num_directional_lights();
		int get_num_spot_lights();
		int get_num_point_lights();
		
		

		
	};
}