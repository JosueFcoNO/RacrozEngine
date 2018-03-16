#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_UTILITY_EXPORT DepthStencyl
		{
		public:
			TextureCore2D* GetTexture() { return &m_DepthStencylTexture; };
			DepthStencylView* GetDepthStencylView() { return &m_DepthStencylView; };

		private:
			TextureCore2D m_DepthStencylTexture;
			DepthStencylView m_DepthStencylView;
		};
	}

}