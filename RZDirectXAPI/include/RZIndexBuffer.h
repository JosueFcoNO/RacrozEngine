#pragma once

///////////////////////////////////
//////////INDEX BUFFER////////////
///////////////////////////////////

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_EXP IndexBuffer : public BasicBuffer
		{
		public:
			virtual ~IndexBuffer() { };

			bool CreateIndexBuffer(eBUFFER_USAGE usage, bool clearIndexList, GfxCore* gfx);

			bool SetThisIndexBuffer(GfxCore* gfx);
			void AddIndex(uint32 newIndex);

			uint32 GetSize() { return m_IndexSize; };
			int32 GetIndex(int32 index) { return m_IndexList[index]; };

			void ClearIndexList();
			void* GetVectorStart() { return &m_IndexList[0]; };

			int32 m_IndexSize;

		private:
			Vector<uint32> m_IndexList;
		};
	}
}