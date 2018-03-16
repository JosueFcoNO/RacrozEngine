#include <RZUtilityPCH.h>
#include "RZDirectXPCH.h"

namespace rczEngine
{
	namespace Gfx
	{
		Shader::~Shader()
		{
			Destroy();
		}

		void Shader::CreateShader()
		{

		}


		void Shader::Destroy()
		{
			if (m_ShaderBlob)
			{
				if (m_ShaderBlob->Release())
				{
				}
			}
		}
	}
}