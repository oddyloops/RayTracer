#include "rt_camera_near_plane.h"
#include "math_util.h"

using namespace rt_support;

rt_camera_near_plane::rt_camera_near_plane(float_3 eye, float focus, float fov, orthonormal_basis basis, image_spec spec) restrict(amp, cpu)
{
	m_center = eye + focus * basis.get_view_direction();
	m_height = 2 * focus * tanf(0.5 * math_util::deg_to_rad(fov * 0.5));
	m_width = spec.get_aspect_ratio() * m_height;
	m_pixel_width = m_width / spec.get_x_resolution();
	m_pixel_height = m_height / spec.get_y_resolution();

	m_vertices[LL] = m_center - (0.5 * m_width *  basis.get_side_vector()) - (0.5 * m_height * basis.get_up_vector());
	m_vertices[LR] = m_vertices[LL] + m_width * basis.get_side_vector();
	m_vertices[UR] = m_vertices[LR] + m_height * basis.get_up_vector();
	m_vertices[UL] = m_vertices[UR] - m_width * basis.get_side_vector();
}


rt_camera_near_plane& rt_camera_near_plane::operator = (const rt_camera_near_plane& cam) restrict(amp, cpu)
{

	m_center = cam.get_center();
	m_height = cam.get_height();
	m_width = cam.get_width();
	m_pixel_height = cam.get_pixel_width();
	m_pixel_height = cam.get_pixel_height();
	m_vertices[UL] = cam.get_upper_left();
	m_vertices[UR] = cam.get_upper_right();
	m_vertices[LL] = cam.get_lower_left();
	m_vertices[LR] = cam.get_lower_right();
	return *this;
}

float rt_camera_near_plane::get_width() const restrict(amp, cpu)
{
	return m_width;
}

float rt_camera_near_plane::get_height() const restrict(amp, cpu)
{
	return m_height;
}

float rt_camera_near_plane::get_pixel_width() const restrict(amp, cpu)
{
	return m_pixel_width;
}

float rt_camera_near_plane::get_pixel_height() const restrict(amp, cpu)
{
	return m_pixel_height;
}

float_3 rt_camera_near_plane::get_center() const restrict(amp, cpu)
{
	return m_center;
}

float_3 rt_camera_near_plane::get_upper_left() const restrict(amp, cpu)
{
	return m_vertices[UL];
}
float_3 rt_camera_near_plane::get_lower_left() const restrict(amp, cpu)
{
	return m_vertices[LL];
}

float_3 rt_camera_near_plane::get_upper_right() const restrict(amp, cpu)
{
	return m_vertices[UR];
}
float_3 rt_camera_near_plane::get_lower_right() const restrict(amp, cpu)
{
	return m_vertices[LR];
}