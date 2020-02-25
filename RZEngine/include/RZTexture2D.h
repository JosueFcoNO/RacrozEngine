#pragma once

namespace rczEngine
{
	class RZ_EXP Texture2D : public Resource
	{
	public:
		Texture2D() { m_Type = ResourceType::RES_TEXTURE; };
		virtual ~Texture2D()
		{
			try
			{
				Release();
			}
			catch (...)
			{
				
			}
		};

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

			auto windowSize = ImVec2(ImGui::GetWindowSize());

			const auto width = m_TextureCore.m_Width;
			const auto height = m_TextureCore.m_Height;

			const auto targetY = windowSize.x * height / width;
			const auto targetX = windowSize.y * width / height;

			if (windowSize.y > targetY)
				windowSize.y = targetY;

			if (windowSize.x > targetX)
				windowSize.x = targetX;

			ImVec2 imagePos = ImGui::GetWindowSize();
			imagePos.x -= windowSize.x;
			imagePos.y -= windowSize.y;

			imagePos.x *= 0.5f;
			imagePos.y *= 0.5f;

			ImGui::SetCursorPos(imagePos);
			ImGui::Image(m_TextureCore.m_ShaderResource, windowSize);
		}
#endif

		virtual void Load(const String& filePath, const String& resName);
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