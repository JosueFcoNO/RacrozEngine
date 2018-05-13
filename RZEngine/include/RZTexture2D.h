#pragma once

namespace rczEngine
{
	class ResVault;

	class RZ_UTILITY_EXPORT Texture2D : public Resource, public Serializable
	{
	public:
		Texture2D() { m_Type = ResourceType::RES_TEXTURE; };
		virtual ~Texture2D() { Release(); };

		uint32 Width();
		uint32 Height();

		Gfx::TextureCore2D* GetTextureCore();
		void SetThisTextureInPS(int startSlot, int NumView, Gfx::GfxCore* gfx);
		void SetThisTextureInVS(int startSlot, int NumView, Gfx::GfxCore* gfx);
		void SetThisTextureInHS(int startSlot, int NumView, Gfx::GfxCore* gfx);
		void SetThisTextureInDS(int startSlot, int NumView, Gfx::GfxCore* gfx);

		void CreateFromRenderTarget(Gfx::RenderTarget& renderTarget);

		virtual void Serialize();
		virtual void DeSerialize();


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

		virtual void Load(const char* filePath, const char* resName);
		virtual void Release() 
		{ 
			if (m_TextureCore.m_ShaderResource)
			m_TextureCore.m_ShaderResource->Release(); 

			if (m_TextureCore.m_Texture)
			m_TextureCore.m_Texture->Release(); 
		};
		
		Gfx::TextureCore2D m_TextureCore;
	};
};