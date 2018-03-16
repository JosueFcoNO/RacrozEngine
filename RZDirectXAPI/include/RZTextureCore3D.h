#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_UTILITY_EXPORT TextureCore3D : public ShaderResource
		{
		public:
			ID3D11Texture3D* m_Texture;
		};
	}
}