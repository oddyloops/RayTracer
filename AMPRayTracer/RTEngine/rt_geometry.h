#pragma once
#include<amp.h>
#include <amp_graphics.h>
#include "ray.h"
#include "intersection_record.h"
#include "indexed_resource.h"
#include "texture_map.h"


using namespace concurrency;
using namespace concurrency::graphics;
using namespace rt_support::ray_support;
using namespace rt_support::scene_resource_support;

namespace rt_support
{
	namespace geometries
	{
		struct rt_geometry_type
		{
			static const int sphere = 1;
			static const int rectangle = 2;
			static const int triangle = 3;
			static const int plane = 4;
			static const int cylinder = 5;
		
		};

		///<summary>
		///Parent class containing basic specifications for a geometry
		///</summary>
		class rt_geometry  : public indexed_resource
		{
		private:
			//rt-geometry
			float_3 m_min;
			float_3 m_max;
			
			//rt-cylinder/sphere
			float_3 m_center;
			float m_radius;
			float m_radius_sq;
			float m_height;
			float_3 m_top_center;
			float_3 m_base_center;
			float_3 m_axis_dir;
			float_3 m_hor_axis_dir;
			float_3 m_hor_axis_dir_perp;
			float m_orth_d; //d for orthogonal plane
			float m_curve_section; //the section of v occupied by curved surface for u-v mapping
			float m_flat_section; //the section of v occupied by a single flat circular surface
			int m_dist_index; //used to determine which part of the cylinder was hit
			
		

			

			//rt-plane/rectangle/triangle
			float_3 point_0;
			float_3 m_true_normal;
			float md, m_map_width, m_map_height;
			float_3 m_u_vec, m_v_vec;
			float_3 m_vertices[4];    // max of 4 vertices
			int m_u_axis_index, m_v_axis_index; // 0 is X, 1 is Y, and 2 is Z
			float m_u_size, m_v_size;
			float ma, mb, mc;
			float md;   // AX + BY + CZ + D = 0, this is the D
			
			//rt-triangle
			float m_apex_u;

			void initialize_triangle();

			void initialize_rectangle();



			

		protected:
			int m_type;
			int m_material_index;
			texture_map<float_3> m_normal_map;
			texture_map<float> m_bump_map;


			

			
		public:

			__declspec(dllexport) rt_geometry();

			__declspec(dllexport) rt_geometry(int geom_type);

			//inits

			//spheres
			__declspec(dllexport) void construct_sphere(float radius, float_3 center);

			__declspec(dllexport) void construct_sphere(float radius, float_3 center, float_3 axis_dir);

			//cylinders
			__declspec(dllexport) void construct_cylinder(float radius, float_3 top_center, float_3 base_center);


			//rectangles
			__declspec(dllexport) void construct_rectangle(float_3 vertices[]);

			__declspec(dllexport) void construct_rectangle(float_3 vertices[], concurrency::array<float, 2> xform);


			//planes
			__declspec(dllexport) void construct_plane(float_3 points[], float map_width, float map_height);


			//triangles
			__declspec(dllexport) void construct_triangle(float_3 vertices[]);

			__declspec(dllexport) void construct_triangle(float_3 vertices[], concurrency::array<float, 2> xform);


			__declspec(dllexport) void set_material_index(int index);


			/// <summary>
			/// Parse command line for <xform> </xform> that is embedeed inside 
			/// Geometry.
			/// </summary>
			static concurrency::array<float,2> parse_xform(float_3 translation, float rx, float ry, float rz, float_3 scale);

			/// <summary>
			/// Returns status of if Ray intersects with this Geom. If so, details of intersection
			/// is returned in the IntersectionRecord.
			/// </summary>
			/// <param name="r">Incoming ray.</param>
			/// <param name="record">If intersect, this record has the details.</param>
			/// <returns>T/F: intersect or not.</returns>
			int intersect(ray& r, intersection_record& record) restrict(amp);

			/// <summary>
			/// Returns status of if Ray intersects with this Geom. If so, details of intersection
			/// is returned in the IntersectionRecord.
			/// </summary>
			/// <param name="r">Incoming ray.</param>
			/// <param name="record">If intersect, this record has the details.</param>
			/// <returns>T/F: intersect or not.</returns>
			int intersect(ray& r, intersection_record& record, texture_view<const float_3, 3> bitmaps, texture_view<const float_3, 1> scalars
				, texture_view<const float, 3> f_bitmaps, texture_view<const float, 1> f_scalars) restrict(amp);

			//get barry-centric coordinates for triangles
			float_3 get_bc(float_3 pt) restrict(amp);

			/// <summary>
			/// Min point
			/// </summary>
			/// <returns></returns>
			float_3 get_min() restrict(amp,cpu);

			/// <summary>
			/// Max point
			/// </summary>
			/// <returns></returns>
			float_3 get_max() restrict(amp,cpu);

			/// <summary>
			/// pt is a position on the geometry, returns the normalized U/V value (between 0 to 1)
			/// No error checking, if pt is not on the geometry, return values are undefined.
			/// </summary>
			/// <param name="pt">position on the geometry (no error checking!)</param>
			/// <param name="bc">barrycentric coordinate of hit point (for triangle only)</param>
			/// <param name="u">returned normalized u value</param>
			/// <param name="v">returned normalized v value</param>
			void get_uv(float_3 pt, float_3 bc, float& u, float& v, int dist_index =-1) restrict(amp);

			/// <summary>
			/// recreives (u,v) and returns the object position that corresponds to the (u,v) coordinate.
			/// </summary>
			/// <param name="u">normalized u</param>
			/// <param name="v">normalized v</param>
			/// <returns>A position that cooresponds to (u,v) on the geometry </returns>
			float_3 get_position(float u, float v) restrict(amp,cpu);

			int get_type() restrict(amp,cpu);

			int get_material_index() restrict(amp,cpu);

			__declspec(dllexport) void set_normal_map(texture_map<float_3> normal_map) restrict(amp, cpu);

			__declspec(dllexport) void set_bump_map(texture_map<float> bump_map) restrict(amp, cpu);

			float_3 get_normal(float u, float v,texture_view<const float_3,3> bitmaps,texture_view<const float_3,1> scalars) restrict(amp);

		};
	}
}
