#include "rt_visibility.h"

using namespace rt;

rt_visibility::rt_visibility() 
{

}

rt_visibility::rt_visibility(scene_database* db)
{
	m_db = db;
}

void rt_visibility::compute_visibility(ray r, int except_geom_index, intersection_record& rec)
{
	for (int i = 0; i < m_db->get_all_spheres().size(); i++)
	{
		if (m_db->get_sphere(i).get_resource_index() != except_geom_index)
		{	
			m_db->get_sphere(i).intersect(r, rec);
		}
	}

	for (int i = 0; i <  m_db->get_all_rectangles().size(); i++)
	{
		if (m_db->get_rectangle(i).get_resource_index() != except_geom_index)
		{
			m_db->get_rectangle(i).intersect(r, rec);
		}
	}
}
