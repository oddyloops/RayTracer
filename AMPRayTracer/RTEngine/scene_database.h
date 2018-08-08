#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include <vector>
#include "rt_sphere.h"
#include "rt_rectangle.h"



using namespace concurrency;
using namespace concurrency::graphics;

using namespace rt_support::geometries;
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
		array_view<rt_sphere, 1> m_spheres;
		array_view<rt_rectangle, 1> m_rects;
	


	public:
		scene_database(array_view<rt_sphere, 1> spheres, array_view<rt_rectangle, 1> rects) restrict(amp,cpu);
		
		

		rt_sphere get_sphere(int index) restrict(amp, cpu);

		rt_rectangle get_rect(int index) restrict(amp, cpu);

		array_view<rt_sphere, 1>& get_all_spheres() restrict(amp, cpu);

		array_view<rt_rectangle, 1>& get_all_rects() restrict(amp, cpu);

		int get_num_spheres() restrict(amp, cpu);
		
		int get_num_rects() restrict(amp, cpu);

		
	};
}