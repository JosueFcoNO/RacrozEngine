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

	void Texture3D::LoadTexture3D(const String& filepath1, const String& filepath2, const String& filepath3, const String& filepath4, const String& resName, bool addToResourceManager)
	{
		m_FilePath = filepath1;
		m_Name = resName;

		//ResVault::Pointer()->InsertResource(shared_from_this());

		//TODO: Change this to more than 4 images.
		Gfx::GfxCore::Pointer()->CreateTexture3DFromFile(filepath1, filepath2, filepath3, filepath4, m_TextureCore, Gfx::USAGE_DEFAULT, Gfx::BIND_SHADER_RESOURCE, Gfx::CPU_DEFAULT);
	}

	void Texture3D::Serialize()
	{
		///TODO:
		auto ser = Serializer::Pointer();
		ser->SetNextObjectSerial(SERIAL_TEXTURE3D);
	}

	void Texture3D::DeSerialize()
	{
	}
}