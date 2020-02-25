#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_EXP DepthStencyl
		{
		public:
			TextureCore2D* GetTexture() { return &m_DepthStencylTexture; };
			DepthStencylView* GetDepthStencylView() { return &m_DepthStencylView; };
			ShaderResource* GetShaderResouceView() { return &m_ShaderResourceView; };

		private:
			TextureCore2D m_DepthStencylTexture;
			DepthStencylView m_DepthStencylView;
			ShaderResource m_ShaderResourceView;
		};
	}

}