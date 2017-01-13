#include "scene_database.h"

using namespace rt_support;

scene_database::scene_database(std::vector<rt_geometry> _geometries, std::vector<rt_material> _materials)
{
	array_view<rt_geometry,1> a_geometries(static_cast<int>(_geometries.size()), _geometries);
}



rt_geometry scene_database::get_geom(int index) restrict(amp, cpu)
{
 }