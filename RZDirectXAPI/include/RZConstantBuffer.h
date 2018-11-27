#pragma once

///////////////////////////////////
//////////CONSTANT BUFFER//////////
///////////////////////////////////

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_EXP ConstantBuffer : public BasicBuffer
		{
		public:
			virtual ~ConstantBuffer() { };

			///Creates a Constant Buffer of size and usage
			bool CreateConstantBuffer(uint32 size, eBUFFER_USAGE usage, GfxCore* gfx);

			///Updates the subresource with srcData
			void UpdateConstantBuffer(void* srcData, GfxCore* gfx);

			///Sets this Buffer in PS
			void SetBufferInPS(uint32 startSlot, GfxCore * gfx);

			///Sets this Buffer in VS
			void SetBufferInVS(uint32 startSlot, GfxCore * gfx);

			///Sets this Buffer in HS
			void SetBufferInHS(uint32 startSlot, GfxCore * gfx);

			///Sets this Buffer in DS
			void SetBufferInDS(uint32 startSlot, GfxCore * gfx);

			///Sets this Buffer in GS
			void SetBufferInGS(uint32 startSlot, GfxCore * gfx);

		};
	}
};