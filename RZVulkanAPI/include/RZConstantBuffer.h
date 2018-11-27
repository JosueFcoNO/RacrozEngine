#pragma once

///////////////////////////////////
//////////CONSTANT BUFFER//////////
///////////////////////////////////

namespace rczEngine
{
	class RZ_EXP ConstantBuffer : public BasicBuffer
	{
	public:
		///Creates a Constant Buffer of size and usage
		bool CreateConstantBuffer(uint32 size, eBUFFER_USAGE usage, GfxCore* gfx = NULL);
		///Updates the subresource with srcData
		bool UpdateConstantBuffer(void* srcData, GfxCore* gfx = NULL);
		///Sets this Buffer in PS
		bool SetBufferInPS(uint32 startSlot, GfxCore * gfx = NULL);
		///Sets this Buffer in VS
		bool SetBufferInVS(uint32 startSlot, GfxCore * gfx = NULL);

	};
};