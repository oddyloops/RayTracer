#include "scene_resource.h"

using namespace rt_support::scene_resource_support;

scene_resource::scene_resource(int count)
{
	m_count = count;
	m_collection(m_count);
}