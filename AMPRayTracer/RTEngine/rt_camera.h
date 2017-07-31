#pragma once

#include "image_spec.h"
#include "pixel_data.h"
#include "vector_amp.h"
#include "math_util.h"

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
		float_3 m_eye;                   // the eye position
		float_3 m_at;                    // the look at position
		float_3 m_up;                    // upvector
		float m_fov;                     // in degree!!
		float m_focus;                   // place to form the image plane
		float_3 m_view_direction;
		float_3 m_side;

		image_spec m_image_spec;           // image spec
		int m_generation;                // generation value
		int m_ortho_mode_on;              // flags whether orthographic mode is on 

		float_3 m_pixel_dx, m_pixel_dy;
		float_3 m_pixel_origin;
		
	public:
		rt_camera() restrict(amp, cpu);

		rt_camera(float_3 eye, float_3 at,float_3 up,float fov,float focus) restrict(amp,cpu);

		rt_camera& operator=(const rt_camera& cam) restrict(amp, cpu);

	
		// sets the image spec object of camera
		void set_image_spec(image_spec spec) restrict(amp,cpu);

		// sets the generation value for camera
		void set_generation(int generation) restrict(amp, cpu);

		// sets the projection mode for camera
		void set_ortho_mode_on(int ortho_mode_on) restrict(amp, cpu);

		void initialize_image(image_spec& spec) restrict(amp, cpu);

	
		
		float_3 get_eye() const restrict(amp, cpu);
		float_3 get_at() const restrict(amp, cpu);
		float_3 get_up() const restrict(amp, cpu);

		float_3 get_view_dir() const restrict(amp, cpu);
		float_3 get_side() const restrict(amp, cpu);
		float get_fov() const restrict(amp, cpu);
		float get_focus() const restrict(amp, cpu);
		image_spec get_image_spec() const restrict(amp, cpu);
		int get_generation() const restrict(amp, cpu);
		int get_ortho_mode_on() const restrict(amp, cpu);

		float_3 get_pixel_position(float x, float y) restrict(amp);
		float_3 get_pixel_dx() const  restrict(amp);
		float_3 get_pixel_dy() const  restrict(amp);


	};
}

