#pragma once


namespace rczEngine
{
	class RZ_UTILITY_EXPORT MeshAPI
	{
	public:
		MeshAPI() {};
		MeshAPI(uint32 indexOffset, uint32 indexCount, uint32 vertexOffset);
		void Draw(GfxCore * gfx = NULL);

		uint32 m_IndexOffset;
		uint32 m_IndexCount;
		uint32 m_VertexOffset;
	};
};