#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RenderTarget
		{
		public:
			RZ_EXP TextureCore2D* GetTextureCore() { return &m_RenderTargetTexture; };
			RZ_EXP RenderTargetView* GetRenderTargetView() { return &m_RenderTargetView; };
			RZ_EXP const String& GetName() const noexcept { return m_Name; };
			RZ_EXP void SetName(const String& name) noexcept { m_Name = name; };
			String m_Name;

		private:
			TextureCore2D m_RenderTargetTexture;
			RenderTargetView m_RenderTargetView;
		};
	}
}