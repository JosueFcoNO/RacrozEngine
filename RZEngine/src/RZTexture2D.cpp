#include <RZEnginePCH.h>

namespace rczEngine
{
	Texture2D::~Texture2D()
	{
		Release();
	};

	uint32 Texture2D::Width()
	{
		return m_TextureCore.m_Width;
	}
	uint32 Texture2D::Height()
	{
		return m_TextureCore.m_Height;
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
		m_Name = renderTarget.GetName();
		m_FilePath = "RenderTarget_"+m_Name;
		m_TextureCore = *renderTarget.GetTextureCore();
	}

	void Texture2D::Serialize()
	{
		auto ser = Serializer::Pointer();
		//Set the serial
		ser->SetNextObjectSerial(SERIAL_TEXTURE2D);
		//Serialize the string
		ser->SerializeString(m_Name);
		//Serialize the filepath and hash
		m_FilePath.Serialize();
		//Serialize the TextureCore2D.
		ser->WriteData(&m_TextureCore.m_AccessFlags, sizeof(m_TextureCore.m_AccessFlags));
		ser->WriteData(&m_TextureCore.m_BindFlags, sizeof(m_TextureCore.m_BindFlags));
		ser->WriteData(&m_TextureCore.m_Format, sizeof(m_TextureCore.m_Format));
		ser->WriteData(&m_TextureCore.m_Height, sizeof(m_TextureCore.m_Height));
		ser->WriteData(&m_TextureCore.m_Width, sizeof(m_TextureCore.m_Width));
		ser->WriteData(&m_TextureCore.m_TextureBytePitch, sizeof(m_TextureCore.m_TextureBytePitch));
		ser->WriteData(&m_TextureCore.m_TextureBytes, sizeof(m_TextureCore.m_TextureBytes));

		//Serialize the pixel data
		void* ptr = Gfx::GfxCore::Pointer()->GetTexture2DData(m_TextureCore);
		ser->WriteData(ptr, m_TextureCore.m_TextureBytes);
		free(ptr);
	}

	void Texture2D::DeSerialize()
	{
		auto ser = Serializer::Pointer();

		//Serialize the string
		ser->DeSerializeString(m_Name);
		//Serialize the filepath and hash
		m_FilePath.DeSerialize();
		//Serialize the TextureCore2D.
		ser->ReadData(&m_TextureCore.m_AccessFlags, sizeof(m_TextureCore.m_AccessFlags));
		ser->ReadData(&m_TextureCore.m_BindFlags, sizeof(m_TextureCore.m_BindFlags));
		ser->ReadData(&m_TextureCore.m_Format, sizeof(m_TextureCore.m_Format));
		ser->ReadData(&m_TextureCore.m_Height, sizeof(m_TextureCore.m_Height));
		ser->ReadData(&m_TextureCore.m_Width, sizeof(m_TextureCore.m_Width));
		ser->ReadData(&m_TextureCore.m_TextureBytePitch, sizeof(m_TextureCore.m_TextureBytePitch));
		ser->ReadData(&m_TextureCore.m_TextureBytes, sizeof(m_TextureCore.m_TextureBytes));

		//Serialize the pixel data
		void* ptr = malloc(m_TextureCore.m_TextureBytes);
		ser->ReadData(ptr, m_TextureCore.m_TextureBytes);

		Gfx::GfxCore::Pointer()->CreateTexture(ptr, m_TextureCore.m_TextureBytePitch, 1, m_TextureCore, m_TextureCore.m_Width, m_TextureCore.m_Height,
			m_TextureCore.m_BindFlags, m_TextureCore.m_Format, 0);

		free(ptr);
	}

	void Texture2D::Load(const String& filePath, const String& resName)
	{
		m_FilePath = filePath;
		m_Name = resName;
		
		ResVault::Pointer()->InsertResource(StrPtr<Texture2D>(this));

		Gfx::GfxCore::Pointer()->CreateTextureFromFile(filePath, m_TextureCore, Gfx::USAGE_DEFAULT, Gfx::BIND_SHADER_RESOURCE, Gfx::CPU_DEFAULT);
	}

}