#pragma once
#include <RZUtilityPCH.h>
#include "RZDirectXPCH.h"

namespace rczEngine
{
	namespace Gfx
	{

		BasicBuffer::BasicBuffer()
		{
			m_Buffer = NULL;
		}

		BasicBuffer::~BasicBuffer()
		{
			Destroy();
		}

		bool BasicBuffer::Create(uint32 sizeOfElement, uint32 numOfElements, void* arrayOfElements, uint32 elementsPitch, uint32 elementsSlice, eBUFFER_USAGE usage, eBIND_FLAGS bindflags, GfxCore* gfx)
		{
			m_BindFlags = bindflags;
			m_SizeOfElement = sizeOfElement;
			m_NumOfElements = numOfElements;
			m_ElementSlice = elementsSlice;
			m_ElementsPitch = elementsPitch;

			return gfx->CreateBuffer(*this, sizeOfElement, numOfElements, arrayOfElements, elementsPitch, elementsSlice, usage, bindflags);
		}

		void BasicBuffer::Destroy()
		{
			if (m_Buffer)
			{
				m_Buffer->Release();
				m_Buffer = NULL;
			}
		}

	}
}