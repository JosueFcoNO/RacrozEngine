#pragma once

///////////////////////////////////////
///////////////BUFFER//////////////////
///////////////////////////////////////

namespace rczEngine
{
	namespace Gfx
	{
		///A class to wrap graphical buffers.
		class RZ_EXP BasicBuffer
		{
		public:
			///Inits the Buffer
			BasicBuffer();
			///Calls the Destroy Function
			virtual ~BasicBuffer();
			///Creates the Buffer.
			virtual bool Create(uint32 sizeOfElement, uint32 numOfElements, void* arrayOfElements, uint32 elementsPitch, uint32 elementsSlice, eBUFFER_USAGE usage, eBIND_FLAGS bindflags, GfxCore* gfx = nullptr);
			///Releases the Buffer
			virtual void Destroy();

			///A Buffer
			ID3D11Buffer* m_Buffer;
			///The buffer's usage.
			eBUFFER_USAGE m_Usage;
			//the buffer's sizo of element.
			uint32 m_SizeOfElement;
			//the buffer's number of element
			uint32 m_NumOfElements;
			//The buffer's element pitch
			uint32 m_ElementsPitch;
			//The buffer's element slice
			uint32 m_ElementSlice;
			//The buffer's bind flags.
			eBIND_FLAGS m_BindFlags;
		};
	}
}

