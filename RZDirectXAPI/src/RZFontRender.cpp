#include <RZDirectXPCH.h>

namespace rczEngine
{
	namespace Gfx
	{
		void FontRender::InitFont(UNICHAR * fontName, GfxCore * gfx)
		{
			//HRESULT hResult = FW1CreateFactory(FW1_VERSION, &m_pFW1Factory);

			//hResult = m_pFW1Factory->CreateFontWrapper(gfx->m_Device, fontName, &m_pFontWrapper);
		}

		void FontRender::DestroyFont()
		{
			//m_pFontWrapper->Release();
			//m_pFW1Factory->Release();
		}

		void FontRender::DrawString(GfxCore * gfx, UNICHAR * String, int textSize, Vector2 pos)
		{
			////m_pFontWrapper->DrawString(
			////	gfx->m_DeviceContext,
			//	String,// String
			//	textSize,// Font size
			//	pos.m_x,// X position
			//	pos.m_y,// Y position
			//	0xffffffff,// Text color, 0xAaBbGgRr
			//	FW1_TEXT_FLAG::FW1_RESTORESTATE | FW1_TEXT_FLAG::FW1_NOGEOMETRYSHADER// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
			//);
		}
	}
};