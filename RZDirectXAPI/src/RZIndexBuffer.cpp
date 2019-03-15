#pragma once 
#include <RZDirectXPCH.h>
#include "..\include\RZIndexBuffer.h"

namespace rczEngine
{
	namespace Gfx
	{
		bool IndexBuffer::CreateIndexBuffer(eBUFFER_USAGE usage, bool clearIndexList, GfxCore* gfx)
		{
			bool res = Create(sizeof(uint32), uint32(m_IndexList.size()), &m_IndexList[0], 0, 0, usage, eBIND_FLAGS::BIND_INDEX_BUFFER, gfx);
			m_IndexSize = m_IndexList.size();
			
			if (clearIndexList)
				m_IndexList.clear();
			
			return res;
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
	}
};