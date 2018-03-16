#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT CubeMapCore
	{
	public:
		ID3D11Texture2D** GetTexture(int slot);
		ID3D11ShaderResourceView** GetShaderResourceView();

		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_ShaderResource;

	};
}
