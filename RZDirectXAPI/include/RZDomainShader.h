#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_UTILITY_EXPORT DomainShader : public Shader
		{
		public:
			///Inits everything to NULL
			DomainShader();
			///Calls Destroy()
			~DomainShader();
			///Creates and compiles the pixel shader found in pszFileString
			bool CreateDomainShader(const WCHAR* pszFileString, GfxCore* gfx = NULL);
			///Set this pixel shader in the input assembler.
			bool SetThisDomainShader(GfxCore* gfx = NULL);
			///Releases data
			void Destroy();

			///A pointer to a Domain Shader
			ID3D11DomainShader* m_DomainShader;
		};
	}
}