#pragma once

///////////////////////////////////
//////////INDEX BUFFER////////////
///////////////////////////////////

namespace rczEngine
{
	namespace Gfx
	{
		class IndexBuffer : public BasicBuffer
		{
		public:
			RZ_EXP virtual ~IndexBuffer() { };

			RZ_EXP bool CreateIndexBuffer(eBUFFER_USAGE usage, bool clearIndexList, GfxCore* gfx);

			RZ_EXP bool SetThisIndexBuffer(GfxCore* gfx);
			RZ_EXP void AddIndex(uint32 newIndex);

			RZ_EXP uint32 GetSize() { return m_IndexList.size(); };
			RZ_EXP int32 GetIndex(int32 index) { return m_IndexList[index]; };

			RZ_EXP void ClearIndexList();
			RZ_EXP void* GetVectorStart() { return &m_IndexList[0]; };
			uint32 m_IndexSize;

		private:

			Vector<uint32> m_IndexList;
		};
	}
}