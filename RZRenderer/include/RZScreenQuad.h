#pragma once


namespace rczEngine
{
	struct UI_Vertex
	{
		UI_Vertex() : m_Uvs(eInit::Zero), m_Position(eInit::Zero)
		{};
		Vector3 m_Position;
		Vector2 m_Uvs;
	};

	class RZ_EXP ScreenQuad
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