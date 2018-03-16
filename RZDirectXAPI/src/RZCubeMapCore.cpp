#include <RZDirectXPCH.h>

namespace rczEngine
{
	ID3D11Texture2D ** CubeMapCore::GetTexture(int slot)
	{
		return &m_Texture;
	}

	ID3D11ShaderResourceView ** CubeMapCore::GetShaderResourceView()
	{
		return &m_ShaderResource;
	}
}