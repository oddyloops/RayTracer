#pragma once
#include <vector>

#include "rt_sphere.h"
#include "rt_rectangle.h"
#include "rt_triangle.h"
#include "rt_plane.h"
#include "rt_cylinder.h"



using namespace std;
using namespace rt_support::geometries;

namespace rt_support
{
	namespace lights
	{


		struct light_type
		{
			static const int DIRECTIONAL = 1;
			static const int POINT = 2;
			static const int SPOT = 3;
			static const int AREA = 4;
		};

		class rt_light
		{
		protected:
			int m_type;
			vector<float> m_color;
			vector<float> m_direction; //does not apply to point light
			vector<float> m_origin; //does not apply to directional light
			float m_range; //farthest distance light can travel (does not apply to directional light)
			float m_att_frac; //fraction of the range where attenuation starts
			bool m_realistic_att; // a flag to determine if attenuation was linear or cosine based (realistic)
			float compute_attenuation(float hitDist);

		public:
			__declspec(dllexport) rt_light(int type);

			void set_color(vector<float> color);
			void set_direction(vector<float> direction);
			void set_origin(vector<float> origin);
			void set_range(float range);
			void set_att_frac(float att_frac);
			void set_realistic_att(bool realistic_att);

			vector<float> get_color() const;
			vector<float> get_direction() const;
			vector<float> get_origin() const;
			float get_range() const;
			float get_att_frac() const;
			bool get_is_realistic_att() const;

			///<summary>
			///Determines the percentage of light a pixel gets based on intersection with the light ray (GPU hates virtual functions)
			///</summary>
			float percent_light(vector<rt_rectangle>& rects, vector<rt_sphere>& spheres, vector<rt_triangle>& triangles, vector<rt_plane>& planes, 
				vector<rt_cylinder>& cylinders, vector<float> geomPoint, int exceptGeomIndex);
		
		
	};



	}
}