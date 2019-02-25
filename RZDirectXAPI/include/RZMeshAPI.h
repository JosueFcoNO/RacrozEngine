#pragma once


namespace rczEngine
{
	namespace Gfx
	{
		class RZ_EXP MeshAPI
		{
		public:
			MeshAPI() {};
			MeshAPI(uint32 indexOffset, uint32 indexCount, uint32 vertexOffset);
			void Draw(GfxCore * gfx = nullptr);

			uint32 m_IndexOffset;
			uint32 m_IndexCount;
			uint32 m_VertexOffset;
		};
	}
};