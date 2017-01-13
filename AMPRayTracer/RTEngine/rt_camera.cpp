#include "rt_camera.h"


using namespace rt_support;

rt_camera::rt_camera(float_3 eye, float_3 at, float_3 up, float fov, float focus) restrict(amp,cpu)
{
	m_eye = eye;
	m_at = at;
	m_up = up;
	m_fov = fov;
	m_focus = focus;

	compute_orthonormal_basis();
	compute_near_plane();
	
}


rt_camera& rt_camera::operator=(const rt_camera& cam) restrict(amp, cpu)
{
	m_eye = cam.get_eye();
	m_at = cam.get_at();
	m_up = cam.get_up();
	m_fov = cam.get_fov();
	m_focus = cam.get_focus();
	m_basis = cam.get_orthonormal_basis();
	m_near_plane = cam.get_rt_camera_near_plane();
	m_generation = cam.get_generation();
	m_ortho_mode_on = cam.get_ortho_mode_on();
	m_image_spec = cam.get_image_spec();

	return *this;
}

// sets the image spec object of camera
void rt_camera::set_image_spec(image_spec spec) restrict(amp, cpu)
{
	m_image_spec = spec;
}

// sets the generation value for camera
void rt_camera::set_generation(int generation) restrict(amp, cpu)
{
	m_generation = generation;
}

void rt_camera::set_ortho_mode_on(int ortho_mode_on) restrict(amp, cpu)
{
	m_ortho_mode_on = ortho_mode_on;
}

void rt_camera::compute_near_plane() restrict(amp, cpu)
{
	m_near_plane = rt_camera_near_plane(m_eye, m_focus, m_fov, m_basis, m_image_spec);
}

void rt_camera::compute_orthonormal_basis() restrict(amp, cpu)
{
	m_basis = orthonormal_basis(m_eye, m_at, m_up);
}


float_3 rt_camera::get_eye() const restrict(amp, cpu)
{
	return m_eye;
}

float_3 rt_camera::get_at() const restrict(amp, cpu)
{
	return m_at;
}

float_3 rt_camera::get_up() const restrict(amp, cpu)
{
	return m_up;
}

float rt_camera::get_fov() const restrict(amp, cpu)
{
	return m_fov;
}


float rt_camera::get_focus() const restrict(amp, cpu)
{
	return m_focus;
}

orthonormal_basis rt_camera::get_orthonormal_basis() const restrict(amp, cpu)
{
	return m_basis;
}

rt_camera_near_plane rt_camera::get_rt_camera_near_plane() const restrict(amp, cpu)
{
	return m_near_plane;
}

image_spec rt_camera::get_image_spec() const restrict(amp, cpu)
{
	return m_image_spec;
}

int rt_camera::get_generation() const restrict(amp, cpu)
{
	return m_generation;
}

int rt_camera::get_ortho_mode_on() const restrict(amp, cpu)
{
	return m_ortho_mode_on;
}
