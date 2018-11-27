#pragma once

namespace rczEngine
{
	class RZ_EXP RenderTarget
	{
	public:
		TextureCore2D* GetTextureCore() { return &m_RenderTargetTexture; };
		RenderTargetView* GetRenderTargetView() { return &m_RenderTargetView; };

		String m_Name;

	private:
		TextureCore2D m_RenderTargetTexture;
		RenderTargetView m_RenderTargetView;
	};

}