#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		///A class to wrap shaders, is a friend to GfxCore.
		class RZ_EXP HullShader : public Shader
		{
		public:
			///Inits everything to nullptr
			HullShader();
			///Calls Destroy()
			~HullShader();
			///Creates and compiles the pixel shader found in pszFileString
			bool CreateHullShader(const WCHAR* pszFileString, GfxCore* gfx = nullptr);
			///Set this pixel shader in the input assembler.
			bool SetThisHullShader(GfxCore* gfx = nullptr);
			///Releases data
			void Destroy();

			///A pointer to a Hull Shader
			ID3D11HullShader* m_HullShader;
		};
	}
}