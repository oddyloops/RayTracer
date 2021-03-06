#pragma once


namespace rt_support
{
	namespace scene_resource_support
	{
		/// <summary>
		/// Base class for all resources of the RT scene database. All resoruces has an index.
		/// </summary>

		class indexed_resource
		{
		private:
			int m_index = 0;
		public:
			__declspec(dllexport) void set_resource_index(int index);
			int get_resource_index();
		};
	}
}