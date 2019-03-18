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

	void DebuggerLineList::SetLineListIndex(const Vector<Vector3>& linePoints, const Vector<uint32>& indexPoints)
	{
		m_IndexBuffer.ClearIndexList();
		m_VertexBuffer.ClearVertexList();
		m_VertexBuffer.Destroy();

		for (auto v : linePoints)
		{
			m_VertexBuffer.AddVertex(v);
		}

		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, Gfx::GfxCore::Pointer());

		UseIndex = true;

		for (auto i : indexPoints)
		{
			m_IndexBuffer.AddIndex(i);
		}

		m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, false, Gfx::GfxCore::Pointer());
	}
};