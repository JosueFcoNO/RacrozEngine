#pragma once
#include <RZDirectXPCH.h>

namespace rczEngine
{
	namespace Gfx
	{
		///A class to wrap shaders, is a friend to GfxCore. Can hold Vertex and Pixel shaders.
		class RZ_EXP Shader
		{
		public:
			///Calls Destroy()
			virtual ~Shader();
			///Inits the object to a pixel shader.
			virtual void CreateShader();
			///Releases data
			virtual void Destroy();

			//The path to the shader
			FilePath ShaderPath;
			///The blob of the compiled shader
			ID3D10Blob* m_ShaderBlob;
		};
	}
}