#include <RZEnginePCH.h>

namespace rczEngine
{	
	Mesh::Mesh(Gfx::MeshAPI meshCore)
	{
		m_MeshCore = meshCore;
	}

	void Mesh::Draw(Gfx::GfxCore* gfx)
	{
		m_MeshCore.Draw(gfx);
	}

}