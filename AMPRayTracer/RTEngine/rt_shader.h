#pragma once




#include "ray.h"
#include "intersection_record.h"
#include "rt_light.h"
#include "rt_material.h"
#include "rt_geometry.h"
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

		float_3 compute_diffuse(intersection_record& rec, rt_material& mat, array_view<rt_light, 1>* lights,  array_view<rt_material, 1>* m_materials, array_view<rt_geometry, 1>* geom,  texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars)  restrict(amp);

		float_3 compute_specular(intersection_record& rec, rt_material& mat, array_view<rt_light, 1>* lights, array_view<rt_material, 1>* m_materials, array_view<rt_geometry, 1>* geom, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars)  restrict(amp);
	
	
		rt_material get_material_from_rec(intersection_record& rec, array_view<rt_material, 1>* materials) restrict(amp);

		float_3 compute_shade_from_ray_dir(int exceptIndex,float_3 origin,float_3 dir, array_view<rt_light, 1>* lights, array_view<rt_material, 1>* m_materials, array_view<rt_geometry, 1>* geom, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalar) restrict(amp);

		float_3 compute_shade_helper(intersection_record& rec, array_view<rt_light, 1>* lights, array_view<rt_material, 1>* m_materials, array_view<rt_geometry, 1>* geom, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp);

	public:
		rt_shader() restrict(amp, cpu);

		rt_shader(float_3 ambient_light, float ambient_intensity, float_3 view_dir) restrict(amp, cpu);
		float_3 compute_shade(intersection_record& rec, int generation,  array_view<rt_light, 1>* lights, array_view<rt_material, 1>* m_materials, array_view<rt_geometry, 1>* geom, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
			, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp);
	};
}