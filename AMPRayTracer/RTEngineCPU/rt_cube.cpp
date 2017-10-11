#include "rt_cube.h"

using namespace rt_support::geometries;

rt_cube::rt_cube() {}


rt_cube::rt_cube(vector<float> min, vector<float> max)
{
	m_min = min;
	m_max = max;
	//base rectangle vertices
	vector<float> vert1[] = { 
		{max[0],min[1],max[2]},
		{ min[0],min[1],max[2]},
		min,
		{max[0],min[1],max[2]}
	};
	m_rects[0] = rt_rectangle(vert1);
	//top rectangle vertices

	vector<float> vert2[] = { 
		{min[0],max[1],max[2]},
		max,
		{max[0],max[1],min[2]},
		{min[0],max[1],min[2]}
	};
	m_rects[1] = rt_rectangle(vert2);

	//left face
	vector<float> vert3[] = {
		min, 
		{min[0],min[1],max[2] },
		{ min[0], max[1],max[2]},
		{ min[0], max[1], min[2]}
	};
	m_rects[2] = rt_rectangle(vert3);

	//right face
	vector<float> vert4[] = {
		{max[0], min[1], max[2]},
		{max[0],min[1],min[2]},
		{max[0], max[1], min[2]},
		max
	};
	m_rects[3] = rt_rectangle(vert4);

	//front face
	vector<float> vert5[] = {
		{min[0],min[1], max[2]},
		{max[0],min[1], max[2]},
		max,
		{min[0],max[1], max[2]}
	};

	m_rects[4] = rt_rectangle(vert5);

	//back face
	vector<float> vert6[] = {
		
		{max[0], min[1],min[2]},
		min,
		{ min[0], max[1],min[2] },
		{max[0], max[1],min[2]}
		
	};
	m_rects[5] = rt_rectangle(vert6);
}

rt_cube::rt_cube(vector<float> min, vector<float> max, matrix xform)
{
	rt_cube(min, max);
	for (int i = 0; i < 6; i++)
	{
		vector<float> vertices[4];
		m_rects[i].get_vertices(vertices);
		m_rects[i] = rt_rectangle(vertices, xform);
	}
}


bool rt_cube::intersect(ray& ray, intersection_record& record)
{
	bool intersects = false;
	for (int i = 0; i < 6; i++)
	{
		intersects = intersects || m_rects[i].intersect(ray, record);
	}
	if (intersects)
	{
		record.set_geom_index(get_resource_index());
		record.set_type(get_type());
		record.set_material_index(get_material_index());
	}
	return intersects;
}


void rt_cube :: get_uv(vector<float> pt, vector<float> bc, float& u, float& v)
{

}


vector<float> rt_cube::get_position(float u, float v)
{
	return { 0 };
}



vector<float> rt_cube::get_max()
{
	return m_max;
}

vector<float> rt_cube::get_min()
{
	return m_min;
}