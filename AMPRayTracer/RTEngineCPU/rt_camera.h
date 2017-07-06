#pragma once

#include "image_spec.h"
#include "pixel_data.h"
#include "orthonormal_basis.h"
#include "rt_camera_near_plane.h"


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
		float m_fov;                     // in degree!!
		float m_focus;                   // place to form the image plane
		orthonormal_basis m_basis;        // orthonormal basis
		image_spec m_image_spec;           // image spec
		rt_camera_near_plane m_near_plane;   // near plane details
		int m_generation;                // generation value
		bool m_ortho_mode_on;              // flags whether orthographic mode is on 

		// create new orthonormal basis 
		// for the camera
		void compute_orthonormal_basis();

		
	public:
		rt_camera();

		rt_camera(vector<float> eye, vector<float> at, vector<float> up,float fov,float focus);

		rt_camera& operator=(const rt_camera& cam);

		// sets the image spec object of camera
		void set_image_spec(image_spec spec);

		// sets the generation value for camera
		void set_generation(int generation);

		// sets the projection mode for camera
		void set_ortho_mode_on(bool ortho_mode_on);

		// calculate the near plane values
		// for the camera
		void compute_near_plane();
		
		vector<float> get_eye() const;
		vector<float> get_at() const ;
		vector<float> get_up() const;
		float get_fov() const;
		float get_focus() const;
		orthonormal_basis get_orthonormal_basis() const;
		rt_camera_near_plane get_rt_camera_near_plane() const;
		image_spec get_image_spec() const;
		int get_generation() const;
		bool get_ortho_mode_on() const;


	};
}

