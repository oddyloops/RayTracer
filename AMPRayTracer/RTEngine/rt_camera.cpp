#include "rt_camera.h"


using namespace rt_support;


rt_camera::rt_camera() restrict(amp, cpu) {}

rt_camera::rt_camera(float_3 eye, float_3 at, float_3 up, float fov, float focus) restrict(amp,cpu)
{
	m_eye = eye;
	m_at = at;
	m_up = up;
	m_fov = fov;
	m_focus = focus;

	m_view_direction = at - eye;

	m_side = vector_amp::cross(m_view_direction, m_up);
	m_up = vector_amp::cross(m_side, m_view_direction);
	m_side = vector_amp::normalize(m_side);
	m_view_direction = vector_amp::normalize(m_view_direction);
	m_up = vector_amp::normalize(m_up);
	
}


rt_camera& rt_camera::operator=(const rt_camera& cam) restrict(amp, cpu)
{
	m_eye = cam.get_eye();
	m_at = cam.get_at();
	m_up = cam.get_up();
	m_fov = cam.get_fov();
	m_focus = cam.get_focus();
	m_side = cam.get_side();
	m_view_direction = cam.get_view_dir();
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

void rt_camera::initialize_image(image_spec& spec) restrict(amp, cpu)
{
	m_image_spec = spec;
	float halfImgHeight = m_focus * tanf(math_util::deg_to_rad(0.5f * m_fov));
	float halfImgWidth = halfImgHeight * spec.get_aspect_ratio();
	float_3 atOnImgPlane = m_eye + (m_focus * m_view_direction);
	m_pixel_origin = atOnImgPlane + (halfImgHeight * m_up) - (halfImgWidth * m_side);
	m_pixel_dx = (halfImgWidth * 2 / (float)spec.get_x_resolution()) * m_side;
	m_pixel_dy = -(halfImgHeight * 2 / (float)spec.get_y_resolution()) * m_up;
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

float_3 rt_camera::get_view_dir() const restrict(amp,cpu)
{
	return m_view_direction;
}

float_3 rt_camera::get_side() const restrict(amp,cpu)
{
	return m_side;
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

float_3 rt_camera::get_pixel_position(float x, float y) restrict(amp)
{
	return m_pixel_origin + (x * m_pixel_dx) + (y * m_pixel_dy);
}

float_3 rt_camera::get_pixel_dx() const restrict(amp)
{
	return m_pixel_dx;
}

float_3 rt_camera::get_pixel_dy() const restrict(amp)
{
	return m_pixel_dy;
}