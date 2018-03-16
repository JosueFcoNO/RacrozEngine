#pragma once

///////////////////////////////////////
///////////////BUFFER//////////////////
///////////////////////////////////////

namespace rczEngine
{	
	///A class to wrap graphical buffers.
	class RZ_UTILITY_EXPORT BasicBuffer
	{
	public:
		///Inits the Buffer
		BasicBuffer();
		///Calls the Destroy Function
		virtual ~BasicBuffer();
		///Creates the Buffer.
		virtual bool Create(uint32 sizeOfElement, uint32 numOfElements, void* arrayOfElements, uint32 elementsPitch, uint32 elementsSlice, eBUFFER_USAGE usage, eBIND_FLAGS bindflags, GfxCore* gfx = NULL);
		///Releases the Buffer
		virtual void Destroy();

		///A Buffer
		ID3D11Buffer* m_Buffer;
		///The buffer's usage.
		eBUFFER_USAGE m_Usage;
	};
}

