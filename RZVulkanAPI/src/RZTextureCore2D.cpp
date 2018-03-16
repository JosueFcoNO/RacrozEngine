#include <RZVulkanPCH.h>
#include <RZTextureCore2D.h>

namespace rczEngine
{
	ID3D11Texture2D ** TextureCore2D::GetTexture()
	{
		return &m_Texture;
	}

	ID3D11ShaderResourceView ** TextureCore2D::GetShaderResourceView()
	{
		return &m_ShaderResource;
	}

}