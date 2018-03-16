#include <RZUtilityPCH.h>
#include "RZVulkanPCH.h"

namespace rczEngine
{

	////////////////////////////////////////////////////////
	//////////////////////    Shader	////////////////////
	////////////////////////////////////////////////////////

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
			m_ShaderBlob->Release();
		}
	}
}