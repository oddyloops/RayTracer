#pragma once
#include <vector>

using namespace std;

namespace rt_support
{
	namespace scene_resource_support
	{

		constexpr float PI = 3.1415926f;

		struct map_type
		{
			static const int plain = 1; 
			static const int stripes = 2;
			static const int checkered = 3;;
			static const int wavy = 4;
		};
		class all_map
		{
		protected:
			bool m_is_file_source;
			int m_map_type;
			bool m_is_null;
			
			//variables required for different mapping types
			vector<float> m_direction;// stripes, and waves (2D)
			float m_stripe_width_perc; //stripes and waves
			float m_x_no, m_y_no; //checkers dimensions
		



		public:
			__declspec(dllexport) all_map();

			__declspec(dllexport) all_map(bool is_file_source, int map_type);

			__declspec(dllexport) void set_direction(vector<float> direction);

			__declspec(dllexport) void set_stripe_width_perc(float stripe_width_perc);

			__declspec(dllexport) void set_checker_dim(float x, float y);

			bool is_null() const;

			all_map& operator=(const all_map& a);


		};
	}
}
