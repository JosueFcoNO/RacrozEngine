#pragma once


namespace rczEngine
{
	namespace Gfx
	{
		///A class to wrap shaders, is a friend to GfxCore. Can hold Vertex and Pixel shaders.
		class RZ_EXP PixelShader : public Shader
		{
		public:
			///Inits everything to NULL
			PixelShader();
			///Calls Destroy()
			~PixelShader();
			///Creates and compiles the pixel shader found in pszFileString
			bool CreatePixelShader(const WCHAR* pszFileString, GfxCore* gfx = NULL);
			///Set this pixel shader in the input assembler.
			void SetThisPixelShader(GfxCore* gfx = NULL);
			///Releases data
			void Destroy();

			///A pointer to a Pixel Shader
			ID3D11PixelShader* m_PixelShader;
		};
	}
}