#pragma once 
#include <RZVulkanPCH.h>
#include "..\include\RZConstantBuffer.h"

namespace rczEngine
{
	bool ConstantBuffer::CreateConstantBuffer(uint32 size, eBUFFER_USAGE usage, GfxCore* gfx)
	{
		return Create(size, 1, NULL, 0, 0, usage, BIND_CONSTANT_BUFFER, gfx);
	}

	bool ConstantBuffer::UpdateConstantBuffer(void* srcData, GfxCore* gfx)
	{	
		gfx->UpdateConstantBuffer(*this, srcData);
		return true;
	}

	bool ConstantBuffer::SetBufferInPS(uint32 startSlot, GfxCore* gfx)
	{
		return gfx->PSSetConstantBuffer(*this, startSlot, 1);
	}

	bool ConstantBuffer::SetBufferInVS(uint32 startSlot, GfxCore* gfx)
	{
		return gfx->VSSetConstantBuffer(*this, startSlot, 1);
	}
};