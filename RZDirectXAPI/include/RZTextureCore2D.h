#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_UTILITY_EXPORT TextureCore2D : public ShaderResource
		{
		public:
			ID3D11Texture2D* m_Texture;
		};
	}
}