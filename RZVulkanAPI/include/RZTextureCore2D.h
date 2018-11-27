#pragma once

namespace rczEngine
{
	class RZ_EXP TextureCore2D
	{
	public:
		ID3D11Texture2D** GetTexture();
		ID3D11ShaderResourceView** GetShaderResourceView();

		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_ShaderResource;
	};
}