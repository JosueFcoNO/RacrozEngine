#include <RZEnginePCH.h>

namespace rczEngine
{
	void Texture3D::SetThisTextureInPS(int startSlot, int NumView, Gfx::GfxCore* gfx)
	{
		gfx->SetPSShaderResource(startSlot, NumView, &m_TextureCore);
	}

	void Texture3D::SetThisTextureInVS(int startSlot, int NumView, Gfx::GfxCore* gfx)
	{
		gfx->SetVSShaderResource(startSlot, NumView, &m_TextureCore);
	}

	void Texture3D::LoadTexture3D(char * filepath1, char * filepath2, char * filepath3, char * filepath4, char * resName, bool addToResourceManager)
	{
		m_FilePath = filepath1;
		m_Name = resName;

		if (addToResourceManager)
		{
			ResVault::Pointer()->InsertResource(this);
		}
		else
		{
			m_Handle = INVALID_RESOURCE;
		}
		//TODO: Change this to more than 4 images.
		Gfx::GfxCore::Pointer()->CreateTexture3DFromFile(filepath1, filepath2, filepath3, filepath4, m_TextureCore, Gfx::USAGE_DEFAULT, Gfx::BIND_SHADER_RESOURCE, Gfx::CPU_DEFAULT);
	}
}