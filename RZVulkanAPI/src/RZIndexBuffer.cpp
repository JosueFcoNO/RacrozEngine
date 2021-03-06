#pragma once 
#include <RZVulkanPCH.h>
#include "..\include\RZIndexBuffer.h"

namespace rczEngine
{
	bool IndexBuffer::CreateIndexBuffer(eBUFFER_USAGE usage, GfxCore* gfx)
	{
		return Create(sizeof(uint32), uint32(m_IndexList.size()), &m_IndexList[0], 0, 0, usage, eBIND_FLAGS::BIND_INDEX_BUFFER, gfx);
	}

	bool IndexBuffer::SetThisIndexBuffer(GfxCore* gfx)
	{
		gfx->SetIndexBuffer(*this, 0);
		return true;
	}

	void IndexBuffer::AddIndex(uint32 newIndex)
	{
		m_IndexList.push_back(newIndex);
	}

	void IndexBuffer::ClearIndexList()
	{
		m_IndexList.clear();
	}
};