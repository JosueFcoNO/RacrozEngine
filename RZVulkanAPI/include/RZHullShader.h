#pragma once

namespace rczEngine
{
	///A class to wrap shaders, is a friend to GfxCore.
	class RZ_UTILITY_EXPORT HullShader : public Shader
	{
	public:
		///Inits everything to NULL
		HullShader();
		///Calls Destroy()
		~HullShader();
		///Creates and compiles the pixel shader found in pszFileString
		bool CreateHullShader(const WCHAR* pszFileString, GfxCore* gfx = NULL);
		///Set this pixel shader in the input assembler.
		bool SetThisHullShader(GfxCore* gfx = NULL);
		///Releases data
		void Destroy();

		///A pointer to a Hull Shader
		ID3D11HullShader* m_HullShader;
	};
}