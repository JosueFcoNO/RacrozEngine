#pragma once
#include <RZDirectXPCH.h>

namespace rczEngine
{
	namespace Gfx
	{
		MeshAPI::MeshAPI(uint32 indexOffset, uint32 indexCount, uint32 vertexOffset)
		{
			m_IndexCount = indexCount;
			m_IndexOffset = indexOffset;
			m_VertexOffset = vertexOffset;
		}

		void MeshAPI::Draw(GfxCore* gfx)
		{
			gfx->DrawIndexed(m_IndexCount, m_IndexOffset, m_VertexOffset);
		}
	}
};
