#pragma once
#include <vector>
#include "indexed_resource.h"
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
			vector<float> m_ambient_color;
			vector<float> m_diffuse_color;
			vector<float> m_specular_color;
			float m_specularity;
		public:
			rt_material(vector<float> ambient_color, vector<float> diffuse_color);

			rt_material(vector<float> ambient_color, vector<float> diffuse_color, vector<float> specular_color, float specularity);

			void set_diffuse(vector<float> color);

			void set_ambience(vector<float> color);

			void set_specular(vector<float> color);

			void set_specular(vector<float> color, float specularity);

			vector<float> get_ambient_color() const;

			vector<float> get_diffuse_color() const;

			vector<float> get_specular_color() const;

			float get_specularity() const;

		};
	}
}