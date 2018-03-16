#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT GeometryShader : public Shader
	{
	public:
		///Inits everything to NULL
		GeometryShader();
		///Calls Destroy()
		~GeometryShader();
		///Creates and compiles the pixel shader found in pszFileString
		bool CreateGeometryShader(const WCHAR* pszFileString, GfxCore* gfx = NULL);
		///Set this pixel shader in the input assembler.
		bool SetThisGeometryShader(GfxCore* gfx = NULL);
		///Releases data
		void Destroy();

		///A pointer to a Domain Shader
		ID3D11GeometryShader* m_GeometryShader;
	};
}