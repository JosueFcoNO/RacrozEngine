#pragma once

namespace rczEngine
{
	class ResVault;

	class RZ_UTILITY_EXPORT Texture2D : public Resource
	{
	public:
		virtual ~Texture2D() { Release(); };

		uint32 Width();
		uint32 Height();

		Gfx::TextureCore2D* GetTextureCore();
		void SetThisTextureInPS(int startSlot, int NumView, Gfx::GfxCore* gfx);
		void SetThisTextureInVS(int startSlot, int NumView, Gfx::GfxCore* gfx);
		void SetThisTextureInHS(int startSlot, int NumView, Gfx::GfxCore* gfx);
		void SetThisTextureInDS(int startSlot, int NumView, Gfx::GfxCore* gfx);

		void CreateFromRenderTarget(Gfx::RenderTarget& renderTarget);


#ifndef RZ_EDITOR
		virtual void RenderResourceGUI()
		{
			ImGui::Text("Texture Resource");
			ImVec2 size = ImGui::GetWindowSize();
			size.x -= 30;
			size.y -= 30;

			ImGui::Image(m_TextureCore.m_ShaderResource, size);
		}
#endif

		virtual void Load(const char* filePath, const char* resName, bool addToResourceManager = true);
		virtual void Release() { m_TextureCore.m_ShaderResource->Release(); m_TextureCore.m_Texture->Release(); };
		
		Gfx::TextureCore2D m_TextureCore;
	};
};