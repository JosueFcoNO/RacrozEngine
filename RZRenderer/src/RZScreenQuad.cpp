#include <RZRendererPCH.h>

namespace rczEngine
{
	void ScreenQuad::InitScreenQuad(Gfx::GfxCore * gfx)
	{
		m_iBuffer.AddIndex(0);
		m_iBuffer.AddIndex(0 + 1);
		m_iBuffer.AddIndex(0 + 2);

		m_iBuffer.AddIndex(0 + 1);
		m_iBuffer.AddIndex(0 + 3);
		m_iBuffer.AddIndex(0 + 2);

		UI_Vertex v;

		v.m_Position.m_x = -1.0f;
		v.m_Position.m_y = -1.0f;
		v.m_Uvs.m_x = 0.0f;
		v.m_Uvs.m_y = 1.0f;

		m_vBuffer.AddVertex(v);

		v.m_Position.m_x = -1.0f;
		v.m_Position.m_y = 1.0f;
		v.m_Uvs.m_x = 0.0f;
		v.m_Uvs.m_y = 0.0f;

		m_vBuffer.AddVertex(v);

		v.m_Position.m_x = 1.0f;
		v.m_Position.m_y = -1.0f;
		v.m_Uvs.m_x = 1.0f;
		v.m_Uvs.m_y = 1.0f;

		m_vBuffer.AddVertex(v);

		v.m_Position.m_x = 1.0f;
		v.m_Position.m_y = 1.0f;
		v.m_Uvs.m_x = 1.0f;
		v.m_Uvs.m_y = 0.0f;

		m_vBuffer.AddVertex(v);

		m_vBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, gfx);
		m_iBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, gfx);
	}

	void ScreenQuad::SetScreenQuadBuffers(Gfx::GfxCore * gfx)
	{
		m_vBuffer.SetThisVertexBuffer(gfx, 1);
		m_iBuffer.SetThisIndexBuffer(gfx);
	}

	void ScreenQuad::DrawScreenQuad(Gfx::GfxCore* gfx)
	{
		gfx->DrawIndexed(6, 1, 0);
	}

}

