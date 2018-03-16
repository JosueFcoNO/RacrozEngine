#pragma once

namespace rczEngine
{
	typedef List<GeometryNode> GeometryTree;

	class RZ_UTILITY_EXPORT OCTREE
	{
	public:
		void InsertModel(Model* model);
		void CalculateOctree(Model* model);

	private:
		GeometryTree m_Tree;
	};
};