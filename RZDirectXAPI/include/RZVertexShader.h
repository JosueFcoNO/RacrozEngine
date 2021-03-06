#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		///A class to wrap shaders, is a friend to GfxCore. 
		class RZ_EXP VertexShader : public Shader
		{
		public:
			///Inits everything to nullptr
			VertexShader();
			///Calls Destroy()
			~VertexShader();
			///Creates and compiles the vertex shader found in pszFileString
			bool CreateVertexShader(const WCHAR* pszFileString, GfxCore* gfx = nullptr);
			///Releases data
			void Destroy();
			///Set This Vertex Shader In The Input Assembly
			void SetThisVertexShader(GfxCore* gfx);
			///Set this Vertex Shader's input layout in the input Assembly
			void SetThisInputLayout(GfxCore* gfx);
			///Set this Vertex Shader and its Input Layout in the pipeline
			void SetThisVertexShaderAndInputLayout(GfxCore* gfx);
			///Reflect this shader's layout.
			void ReflectLayout(int layoutSlot, GfxCore* gfx);

			///A pointer to a Vertex Shader
			ID3D11VertexShader* m_VertexShader;
			InputLayout m_InputLayout;

		};
	}
};