#include <RZEnginePCH.h>

namespace rczEngine
{
	uint32 Texture2D::Width()
	{
		return 1;
	}
	uint32 Texture2D::Height()
	{
		return 1;
	}

	Gfx::TextureCore2D * Texture2D::GetTextureCore()
	{
		return &m_TextureCore;
	}

	void Texture2D::SetThisTextureInPS(int startSlot, int NumView, Gfx::GfxCore * gfx)
	{
		gfx->SetPSShaderResource(startSlot, NumView, &m_TextureCore);
	}

	void Texture2D::SetThisTextureInVS(int startSlot, int NumView, Gfx::GfxCore * gfx)
	{
		gfx->SetVSShaderResource(startSlot, NumView, &m_TextureCore);
	}

	void Texture2D::SetThisTextureInHS(int startSlot, int NumView, Gfx::GfxCore * gfx)
	{
		gfx->SetHSShaderResource(startSlot, NumView, &m_TextureCore);
	}

	void Texture2D::SetThisTextureInDS(int startSlot, int NumView, Gfx::GfxCore * gfx)
	{
		gfx->SetDSShaderResource(startSlot, NumView, &m_TextureCore);
	}

	void Texture2D::CreateFromRenderTarget(Gfx::RenderTarget & renderTarget)
	{
		m_Name = renderTarget.m_Name;
		m_FilePath = "RenderTarget_"+m_Name;
		m_TextureCore = *renderTarget.GetTextureCore();
	}

	void Texture2D::Load(const char* filePath, const char* resName, bool addToResourceManager)
	{
		m_FilePath = filePath;
		m_Name = resName;

		if (addToResourceManager)
		{
			ResVault::Pointer()->InsertResource(this);
		}
		else
		{
			m_Handle = INVALID_RESOURCE;
		}

		Gfx::GfxCore::Pointer()->CreateTextureFromFile(filePath, m_TextureCore, Gfx::USAGE_DEFAULT, Gfx::BIND_SHADER_RESOURCE, Gfx::CPU_DEFAULT);
	}

}