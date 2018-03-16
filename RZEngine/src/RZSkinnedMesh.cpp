#include <RZEnginePCH.h>

namespace rczEngine
{
	SkinnedMesh::SkinnedMesh(Gfx::MeshAPI meshCore)
	{
		m_MeshCore = meshCore;
	}

	void SkinnedMesh::Draw(Gfx::GfxCore* gfx)
	{
		m_MeshCore.Draw(gfx);
	}
};