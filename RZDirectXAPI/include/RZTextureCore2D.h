#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_UTILITY_EXPORT TextureCore2D : public ShaderResource
		{
		public:
			ID3D11Texture2D* m_Texture;

			size_t m_TextureBytePitch;
			size_t m_TextureBytes;
			Gfx::eBIND_FLAGS m_BindFlags;
			int32 m_AccessFlags;
			Gfx::eFORMAT m_Format;
			int32 m_Height;
			int32 m_Width;
		};
	}
}