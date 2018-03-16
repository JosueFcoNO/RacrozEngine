#pragma once


namespace rczEngine
{
	struct UI_Vertex
	{
		UI_Vertex() : m_Uvs(INIT_ZERO), m_Position(INIT_ZERO)
		{};
		Vector3 m_Position;
		Vector2 m_Uvs;
	};

	class RZ_UTILITY_EXPORT ScreenQuad
	{
	public:
		void InitScreenQuad(Gfx::GfxCore * gfx);
		void SetScreenQuadBuffers(Gfx::GfxCore* gfx);
		void DrawScreenQuad(Gfx::GfxCore* gfx);

	private:
		Gfx::VertexBuffer<UI_Vertex> m_vBuffer;
		Gfx::IndexBuffer m_iBuffer;
	};
}