#include "rt_camera.h"


using namespace rt_support;


rt_camera::rt_camera()
{

}

rt_camera::rt_camera(vector<float> eye, vector<float> at, vector<float> up, float fov, float focus)
{
	m_eye = eye;
	m_at = at;
	m_up = up;
	m_fov = fov;
	m_focus = focus;

	compute_orthonormal_basis();
	compute_near_plane();
	
}


rt_camera& rt_camera::operator=(const rt_camera& cam)
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
void rt_camera::set_image_spec(image_spec spec)
{
	m_image_spec = spec;
}

// sets the generation value for camera
void rt_camera::set_generation(int generation)
{
	m_generation = generation;
}

void rt_camera::set_ortho_mode_on(bool ortho_mode_on)
{
	m_ortho_mode_on = ortho_mode_on;
}

void rt_camera::compute_near_plane()
{
	m_near_plane = rt_camera_near_plane(m_eye, m_focus, m_fov, m_basis, m_image_spec);
}

void rt_camera::compute_orthonormal_basis()
{
	m_basis = orthonormal_basis(m_eye, m_at, m_up);
}


vector<float> rt_camera::get_eye() const
{
	return m_eye;
}

vector<float> rt_camera::get_at() const
{
	return m_at;
}

vector<float> rt_camera::get_up() const
{
	return m_up;
}

float rt_camera::get_fov() const
{
	return m_fov;
}


float rt_camera::get_focus() const
{
	return m_focus;
}

orthonormal_basis rt_camera::get_orthonormal_basis() const
{
	return m_basis;
}

rt_camera_near_plane rt_camera::get_rt_camera_near_plane() const
{
	return m_near_plane;
}

image_spec rt_camera::get_image_spec() const
{
	return m_image_spec;
}

int rt_camera::get_generation() const
{
	return m_generation;
}

bool rt_camera::get_ortho_mode_on() const
{
	return m_ortho_mode_on;
}
