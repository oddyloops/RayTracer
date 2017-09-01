#pragma once

#include <vector>

#include "image_spec.h"


using namespace std;

namespace rt_support
{
	/// <summary>
	/// Camera that views the RT scene. Constrcuts based on command file viewing parameters:
	///         eye
	///         lookat
	///         upvector
	///         fov
	///         focus
	/// After the constructor, you should put in initialization computation to compute an
	/// orthogonal frame for this camera (and anything else you deem important) to support RT
	/// run time.
	/// 
	/// In addition, you may want to include functionality to support computation of pixel location 
	/// based on pixel localIndexBuffer (i,j) [You will need ImageSpec information in order to perform this 
	/// computation].
	/// </summary>

	class rt_camera
	{
	private:
		vector<float> m_eye;                   // the eye position
		vector<float> m_at;                    // the look at position
		vector<float> m_up;                    // upvector
		vector<float> m_view_direction;
		vector<float> m_side;
		float m_fov;                     // in degree!!
		float m_focus;                   // place to form the image plane
		image_spec m_image_spec;           // image spec
		int m_generation;                // generation value
		bool m_ortho_mode_on;              // flags whether orthographic mode is on 
	
		vector<float> m_pixel_dx,m_pixel_dy;
		vector<float> m_pixel_origin;
		
	public:
		__declspec(dllexport) rt_camera();

		__declspec(dllexport) rt_camera(vector<float> eye, vector<float> at, vector<float> up,float fov,float focus);

		__declspec(dllexport) rt_camera& operator=(const rt_camera& cam);

		// sets the image spec object of camera
		void set_image_spec(image_spec spec);

		// sets the generation value for camera
		void set_generation(int generation);

		// sets the projection mode for camera
		__declspec(dllexport) void set_ortho_mode_on(bool ortho_mode_on);

		void initialize_image(image_spec& spec);
	
		
		vector<float> get_eye() const;
		vector<float> get_at() const ;
		vector<float> get_up() const;
		vector<float> get_view_dir() const;
		vector<float> get_side() const;

		float get_fov() const;
		float get_focus() const;
		image_spec get_image_spec() const;
		int get_generation() const;
		bool get_ortho_mode_on() const;
		vector<float> get_pixel_position(float x, float y);
		vector<float> get_pixel_dx() const;
		vector<float> get_pixel_dy() const;

	};
}

