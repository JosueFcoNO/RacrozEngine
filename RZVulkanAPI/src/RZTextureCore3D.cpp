#include <RZVulkanPCH.h>

namespace rczEngine
{
	ID3D11Texture3D ** TextureCore3D::GetTexture()
	{
		return &m_Texture;
	}

	ID3D11ShaderResourceView ** TextureCore3D::GetShaderResourceView()
	{
		return &m_ShaderResource;
	}

}