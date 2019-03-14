#include <RZEnginePCH.h>

namespace rczEngine
{
	void DebuggerLineList::SetLineList(const Vector<Vector3>& linePoints)
	{
		m_VertexBuffer.ClearVertexList();
		m_VertexBuffer.Destroy();

		for (auto v : linePoints)
		{
			m_VertexBuffer.AddVertex(v);
		}

		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, Gfx::GfxCore::Pointer());
	}
};