#pragma once

namespace rczEngine
{
	class RZ_EXP TextureCore3D
	{
	public:
		ID3D11Texture3D** GetTexture();
		ID3D11ShaderResourceView** GetShaderResourceView();

		ID3D11Texture3D* m_Texture;
		ID3D11ShaderResourceView* m_ShaderResource;
	};
}