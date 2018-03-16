#include <RZEnginePCH.h>

namespace rczEngine
{
	void CubeMap::LoadCubeMapFromDDS(const char * filePath, const char * resName, bool addToResManager)
	{
		m_FilePath = filePath;
		m_Name = resName;

		if (addToResManager)
		{
			ResVault::Pointer()->InsertResource(this);
		}
		else
		{
			m_Handle = INVALID_RESOURCE;
		}

		Gfx::GfxCore::Pointer()->CreateCubeMapFromDDS(filePath, m_TextureCore);
	}

	void CubeMap::LoadCubeMapFrom6Images(char * front, char * back, char * left, char * right, char * top, char * down, char * resName, bool addToResManager)
	{
	}
};