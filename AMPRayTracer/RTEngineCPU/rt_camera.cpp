#include "rt_camera.h"
#include "vector_util.h"
#include "math_util.h"


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
	m_view_direction = at - eye;

	m_side = vector_util::cross(m_view_direction, m_up);
	m_up = vector_util::cross(m_side, m_view_direction);
	m_side = vector_util::normalize(m_side);
	m_view_direction = vector_util::normalize(m_view_direction);
	m_up = vector_util::normalize(m_up);
	
}


rt_camera& rt_camera::operator=(const rt_camera& cam)
{
	m_eye = cam.get_eye();
	m_at = cam.get_at();
	m_up = cam.get_up();
	m_fov = cam.get_fov();
	m_focus = cam.get_focus();
	m_generation = cam.get_generation();
	m_ortho_mode_on = cam.get_ortho_mode_on();
	m_image_spec = cam.get_image_spec();
	m_side = cam.get_side();
	m_view_direction = cam.get_view_dir();

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

vector<float> rt_camera::get_view_dir() const
{
	return m_view_direction;
}

vector<float> rt_camera::get_side() const
{
	return m_side;
}

float rt_camera::get_fov() const
{
	return m_fov;
}


float rt_camera::get_focus() const
{
	return m_focus;
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


void rt_camera::initialize_image(image_spec& spec)
{
	m_image_spec = spec;
	float halfImgHeight = m_focus * tanf(math_util::deg_to_rad(0.5f * m_fov));
	float halfImgWidth = halfImgHeight * spec.get_aspect_ratio();
	vector<float> atOnImgPlane = m_eye + (m_focus * m_view_direction);
	m_pixel_origin = atOnImgPlane + (halfImgHeight * m_up) - (halfImgWidth * m_side);
	m_pixel_dx = (halfImgWidth * 2 / (float)spec.get_x_resolution()) * m_side;
	m_pixel_dy = -(halfImgHeight * 2 / (float)spec.get_y_resolution()) * m_up;
}

vector<float> rt_camera::get_pixel_position(float x, float y)
{
	return m_pixel_origin + (x * m_pixel_dx) + (y * m_pixel_dy);
}

vector<float> rt_camera::get_pixel_dx() const
{
	return m_pixel_dx;
}

vector<float> rt_camera::get_pixel_dy() const
{
	return m_pixel_dy;
}