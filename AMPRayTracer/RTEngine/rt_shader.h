#pragma once




#include "ray.h"
#include "intersection_record.h"
#include "rt_directional_light.h"
#include "rt_point_light.h"
#include "rt_spot_light.h"
#include "rt_area_light.h"
#include "rt_material.h"
#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "rt_triangle.h"
#include "rt_cylinder.h"
#include "rt_plane.h"
#include "pixel_data.h"
#include "rt_visibility.h"


using namespace std;


using namespace rt_support;
using namespace rt_support::lights;
using namespace rt_support::geometries;
using namespace rt_support::ray_support;
using namespace rt_support::scene_resource_support;


namespace rt
{
	
	///<summary>
	///Handles all shading computation
	///</summary>
	class rt_shader
	{

		float_3 m_ambient_light;
		float m_ambient_intensity;
		float_3 m_view_dir;
		rt_visibility m_visibility;

		float_3 compute_ambience(rt_material& mat,  texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars, intersection_record& rec) restrict(amp);

		float_3 compute_diffuse(intersection_record& rec, rt_material& mat, array_view<rt_directional_light, 1>* m_dir_lights, array_view<rt_point_light, 1>* m_point_lights, array_view<rt_area_light, 1>* m_area_lights,
			array_view<rt_spot_light, 1>* m_spot_lights, array_view<rt_material, 1>* m_materials, array_view<rt_rectangle, 1>* m_rectangles, array_view<rt_sphere, 1>* m_spheres,
			array_view<rt_triangle,1>* m_triangles,array_view<rt_plane,1>* m_planes, array_view<rt_cylinder,1>* m_cylinders, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars)  restrict(amp);

		float_3 compute_specular(intersection_record& rec, rt_material& mat, array_view<rt_directional_light, 1>* m_dir_lights, array_view<rt_point_light, 1>* m_point_lights, array_view<rt_area_light, 1>* m_area_lights,
			array_view<rt_spot_light, 1>* m_spot_lights, array_view<rt_material, 1>* m_materials, array_view<rt_rectangle, 1>* m_rectangles, array_view<rt_sphere, 1>* m_spheres,
			array_view<rt_triangle, 1>* m_triangles, array_view<rt_plane, 1>* m_planes, array_view<rt_cylinder, 1>* m_cylinders, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars)  restrict(amp);
	
	
		rt_material get_material_from_rec(intersection_record& rec, array_view<rt_material, 1>* materials) restrict(amp);

		float_3 compute_shade_from_ray_dir(int exceptIndex,float_3 origin,float_3 dir, array_view<rt_directional_light, 1>* m_dir_lights, array_view<rt_point_light, 1>* m_point_lights, array_view<rt_area_light, 1>* m_area_lights,
			array_view<rt_spot_light, 1>* m_spot_lights, array_view<rt_material, 1>* m_materials, array_view<rt_rectangle, 1>* m_rectangles, array_view<rt_sphere, 1>* m_spheres,
			array_view<rt_triangle, 1>* m_triangles, array_view<rt_plane, 1>* m_planes, array_view<rt_cylinder, 1>* m_cylinders, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp);

		float_3 compute_shade_helper(intersection_record& rec,  array_view<rt_directional_light, 1>* m_dir_lights, array_view<rt_point_light, 1>* m_point_lights, array_view<rt_area_light, 1>* m_area_lights,
			array_view<rt_spot_light, 1>* m_spot_lights, array_view<rt_material, 1>* m_materials, array_view<rt_rectangle, 1>* m_rectangles, array_view<rt_sphere, 1>* m_spheres,
			array_view<rt_triangle, 1>* m_triangles, array_view<rt_plane, 1>* m_planes, array_view<rt_cylinder, 1>* m_cylinders, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp);

	public:
		rt_shader() restrict(amp, cpu);

		rt_shader(float_3 ambient_light, float ambient_intensity, float_3 view_dir) restrict(amp, cpu);
		float_3 compute_shade(intersection_record& rec, int generation,  array_view<rt_directional_light, 1>* m_dir_lights, array_view<rt_point_light, 1>* m_point_lights, array_view<rt_area_light, 1>* m_area_lights,
			array_view<rt_spot_light, 1>* m_spot_lights, array_view<rt_material, 1>* m_materials, array_view<rt_rectangle, 1>* m_rectangles, array_view<rt_sphere, 1>* m_spheres,
			array_view<rt_triangle, 1>* m_triangles, array_view<rt_plane, 1>* m_planes, array_view<rt_cylinder, 1>* m_cylinders, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp);
	};
}