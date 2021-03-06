#pragma once

namespace rczEngine
{
	///A class to wrap shaders, is a friend to GfxCore. 
	class RZ_EXP VertexShader : public Shader
	{
	public:
		///Inits everything to NULL
		VertexShader();
		///Calls Destroy()
		~VertexShader();
		///Creates and compiles the vertex shader found in pszFileString
		bool CreateVertexShader(const WCHAR* pszFileString, GfxCore* gfx = NULL);
		///Releases data
		void Destroy();
		///Set This Vertex Shader In The Input Assembly
		bool SetThisVertexShader(GfxCore* gfx = NULL);
		///Set this Vertex Shader's input layout in the input Assembly
		bool SetThisInputLayout(GfxCore* gfx = NULL);
		///Set this Vertex Shader and its Input Layout in the pipeline
		bool SetThisVertexShaderAndInputLayout(GfxCore* gfx = NULL);
		///Reflect this shader's layout.
		void ReflectLayout(GfxCore* gfx = NULL);

		///A pointer to a Vertex Shader
		ID3D11VertexShader* m_VertexShader;
		InputLayout m_InputLayout;

	};
};