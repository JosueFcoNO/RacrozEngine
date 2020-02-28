#pragma once 
#include <RZDirectXPCH.h>
#include "..\include\RZConstantBuffer.h"

namespace rczEngine
{
	namespace Gfx
	{
		bool ConstantBuffer::CreateConstantBuffer(uint32 size, eBUFFER_USAGE usage, GfxCore* gfx)
		{
			return Create(size, 1, nullptr, 0, 0, usage, BIND_CONSTANT_BUFFER, gfx);
		}

		void ConstantBuffer::UpdateConstantBuffer(void* srcData, GfxCore* gfx)
		{
			gfx->UpdateConstantBuffer(*this, srcData);
		}

		void ConstantBuffer::SetBufferInPS(uint32 startSlot, GfxCore* gfx)
		{
			gfx->PSSetConstantBuffer(*this, startSlot, 1);
		}

		void ConstantBuffer::SetBufferInVS(uint32 startSlot, GfxCore* gfx)
		{
			gfx->VSSetConstantBuffer(*this, startSlot, 1);
		}

		void ConstantBuffer::SetBufferInHS(uint32 startSlot, GfxCore* gfx)
		{
			gfx->HSSetConstantBuffer(*this, startSlot, 1);
		}

		void ConstantBuffer::SetBufferInDS(uint32 startSlot, GfxCore* gfx)
		{
			gfx->DSSetConstantBuffer(*this, startSlot, 1);
		}

		void ConstantBuffer::SetBufferInGS(uint32 startSlot, GfxCore* gfx)
		{
			gfx->GSSetConstantBuffer(*this, startSlot, 1);
		}
	}

};