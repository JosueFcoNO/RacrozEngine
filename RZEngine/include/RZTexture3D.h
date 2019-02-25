#pragma once


namespace rczEngine
{
	class RZ_EXP Texture3D : public Resource
	{
	public:
		Texture3D() { m_Type = ResourceType::RES_TEXTURE; };
		virtual ~Texture3D() { Release(); };

		uint32 Width() { return 1; };
		uint32 Height() { return 2; };
		uint32 Depth() { return 3; };

		Gfx::TextureCore3D* GetTextureCore() { return &m_TextureCore; };
		FORCEINLINE void SetThisTextureInPS(int startSlot, int NumView, Gfx::GfxCore* gfx);
		FORCEINLINE void SetThisTextureInVS(int startSlot, int NumView, Gfx::GfxCore* gfx);

		void LoadTexture3D(const String& filepath1, const String& filepath2, const String& filepath3, const String& filepath4, const String& resName, bool addToResourceManager = true);

		virtual void Load(const String& filePath, const String& resName) {};
		virtual void Release() { m_TextureCore.m_ShaderResource->Release(); m_TextureCore.m_Texture->Release(); };

		virtual void Serialize();
		virtual void DeSerialize();

#ifndef RZ_EDITOR
		virtual void RenderResourceGUI()
		{
			ImGui::Text("Material Resource");
		}
#endif

		Gfx::TextureCore3D m_TextureCore;
	};
};