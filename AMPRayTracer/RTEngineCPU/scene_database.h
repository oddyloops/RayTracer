#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include <vector>
#include "rt_geometry.h"
#include "rt_material.h"



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
		array_view<rt_geometry, 1> m_geometries;
		array_view<rt_material, 1> m_materials;


	public:
		scene_database(std::vector<rt_geometry> _geometries, std::vector<rt_material> _materials);

		/// <summary>
		/// Geometry: Can be access as an array, the collection is a simple array
		/// </summary>
		void add_geom(rt_geometry g);

		rt_geometry get_geom(int index) restrict(amp, cpu);

		array_view<rt_geometry, 1> get_all_geom() restrict(amp, cpu);

		int get_num_geom() restrict(amp, cpu);
		

		/// <summary>
		/// Material: can only be access given an index (the collection may be a sparse array!)
		/// </summary>
		/// <param name="m"></param>
		void add_material(rt_material rm);

		rt_material get_material(int index) restrict(amp, cpu);

		array_view<rt_material, 1> get_all_material() restrict(amp, cpu);

		int get_num_material() restrict(amp, cpu);
	};
}