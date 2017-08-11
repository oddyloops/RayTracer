#pragma once
#include <vector>

using namespace std;

namespace rt_support
{
	namespace scene_resource_support
	{
		///<summary>
		///Encapsulates all information regarding geometry surfaces
		///</summary>
		class rt_material
		{
		private:
			vector<float> m_diffuse;
			vector<float> m_specular;

		public:
			rt_material();
			rt_material(vector<float> diffuse, vector<float> specular);

			vector<float> get_diffuse();
			vector<float> get_specular();
		};
	}
}