#include <RZEnginePCH.h>

namespace rczEngine
{
	void CubeMap::LoadCubeMapFromDDS(const char * filePath, const char * resName)
	{
		m_FilePath = filePath;
		m_Name = resName;

		ResVault::Pointer()->InsertResource(shared_from_this());

		Gfx::GfxCore::Pointer()->CreateCubeMapFromDDS(filePath, m_TextureCore);
	}

	void CubeMap::LoadCubeMapFrom6Images(char * front, char * back, char * left, char * right, char * top, char * down, char * resName)
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