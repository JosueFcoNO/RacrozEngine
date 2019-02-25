#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_EXP DomainShader : public Shader
		{
		public:
			///Inits everything to nullptr
			DomainShader();
			///Calls Destroy()
			~DomainShader();
			///Creates and compiles the pixel shader found in pszFileString
			bool CreateDomainShader(const WCHAR* pszFileString, GfxCore* gfx = nullptr);
			///Set this pixel shader in the input assembler.
			bool SetThisDomainShader(GfxCore* gfx = nullptr);
			///Releases data
			void Destroy();

			///A pointer to a Domain Shader
			ID3D11DomainShader* m_DomainShader;
		};
	}
}