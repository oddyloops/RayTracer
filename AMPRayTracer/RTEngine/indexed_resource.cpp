#include "indexed_resource.h"

using namespace rt_support::scene_resource_support;

void indexed_resource::set_resource_index(int index)
{
	this->m_index = index;
}

int indexed_resource::get_resource_index()
{
	return this->m_index;
}