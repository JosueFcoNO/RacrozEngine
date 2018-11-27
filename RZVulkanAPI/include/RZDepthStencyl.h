#pragma once

namespace rczEngine
{
	class RZ_EXP DepthStencyl
	{
	public:
		TextureCore2D* GetTexture() { return &m_DepthStencylTexture; };
		DepthStencylView* GetDepthStencylView() { return &m_DepthStencylView; };

	private:
		TextureCore2D m_DepthStencylTexture;
		DepthStencylView m_DepthStencylView;
	};

}