#include <RZEnginePCH.h>

namespace rczEngine
{
	void CubeMap::LoadCubeMapFromDDS(const String& filePath, const String& resName)
	{
		m_FilePath = filePath;
		m_Name = resName;

		ResVault::Pointer()->InsertResource(StrPtr<CubeMap>(this));

		Gfx::GfxCore::Pointer()->CreateCubeMapFromDDS(filePath, m_TextureCore);
	}

	void CubeMap::LoadCubeMapFrom6Images(const String& front, const String& back, const String& left, const String& right, const String&top, const String& down, const String& resName)
	{
	}

	void CubeMap::Serialize()
	{
		auto ser = Serializer::Pointer();
		ser->SetNextObjectSerial(SERIAL_CUBEMAP);
	}

	void CubeMap::DeSerialize()
	{

	}
};