#pragma once
#include <vector>
#include "indexed_resource.h"
#include "float_map.h"
#include "vector_map.h"
using namespace std;

namespace rt_support
{
	namespace scene_resource_support
	{
		///<summary>
		///Encapsulates all information regarding geometry surfaces
		///</summary>
		class rt_material : public indexed_resource
		{
		private:
			vector_map m_ambient_color;
			vector_map m_diffuse_color;
			vector_map m_specular_color;
			float_map m_specularity;
			float_map m_refractive_index;
			float_map m_transparency;
			float_map m_reflectivity;
		public:
			__declspec(dllexport) rt_material();

			__declspec(dllexport) rt_material(vector_map ambient_color, vector_map diffuse_color);

			__declspec(dllexport) rt_material(vector_map ambient_color, vector_map diffuse_color, vector_map specular_color, float_map specularity);

			__declspec(dllexport) void set_ref_properties(float_map ref_index,float_map transparency,float_map reflectivity);


			void set_diffuse(vector_map color);

			void set_ambience(vector_map color);

			void set_specular(vector_map color);

			void set_specular(vector_map color, float_map specularity);

			vector<float> get_ambient_color(float u, float v);

			vector<float> get_diffuse_color(float u, float v);

			vector<float> get_specular_color(float u, float v);

			float get_specularity(float u, float v);

			float get_refractive_index(float u, float v);

			float get_transparency(float u, float v);

			float get_reflectivity(float u, float v);

			bool get_is_specular();

			bool get_is_reflective_refractive();


		};
	}
}