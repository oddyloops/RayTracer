#pragma once

#include <vector>
#include "rt_sphere.h"
#include "rt_rectangle.h"


using namespace rt_support::geometries;


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
	

	public:
		scene_database(std::vector<rt_sphere> _spheres, std::vector<rt_rectangle> _rects);

		void add_sphere(rt_sphere s);
		void add_rect(rt_rectangle r);

		rt_sphere get_sphere(int index);
		rt_rectangle get_rectangle(int index);
		
		vector<rt_sphere> get_all_spheres() const;
		vector<rt_rectangle> get_all_rectangles() const;

		int get_num_spheres();
		int get_num_rects();
		
		

		
	};
}