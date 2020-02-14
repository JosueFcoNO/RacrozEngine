#pragma once

namespace rczEngine
{
	class RZ_EXP RenderTexture : public Texture2D
	{
	public:
		RenderTexture() { m_Type = ResourceType::RES_RENDER_TEXTURE; };


		gsl::not_null<Gfx::RenderTarget> GetRenderTarget();

#ifndef RZ_EDITOR
		virtual void RenderResourceGUI()
		{
			ImGui::Text("Render Texture Resource");
			ImVec2 size = ImGui::GetWindowSize();
			size.x -= 30;
			size.y -= 30;

			ImGui::Image(m_TextureCore.m_ShaderResource, size);
		}
#endif

		virtual void Load(const String& filePath, const String& resName) {};

		virtual void Release()
		{
			Texture2D::Release();
			
			if (m_RenderTarget.GetRenderTargetView())
				m_RenderTarget.GetRenderTargetView()->m_RenderTargetView->Release();
		};

		Gfx::RenderTarget m_RenderTarget;
	};
}