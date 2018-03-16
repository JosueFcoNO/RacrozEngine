#include <RZUtilityPCH.h>
#include "RZVulkanPCH.h"

namespace rczEngine
{
	PixelShader::PixelShader()
	{
		m_PixelShader = NULL;
		m_ShaderBlob = NULL;
	}

	PixelShader::~PixelShader()
	{
		Destroy();
	}

	bool PixelShader::CreatePixelShader(const WCHAR * pszFileString, GfxCore* gfx)
	{
		return gfx->CompileAndCreatePixelShader(*this, pszFileString);
	}

	bool PixelShader::SetThisPixelShader(GfxCore* gfx)
	{
		if (m_PixelShader == NULL || m_ShaderBlob == NULL)
		return false;

		return gfx->SetPixelShader(*this);
	}

	void PixelShader::Destroy()
	{
		if (m_PixelShader) m_PixelShader->Release();
		if (m_ShaderBlob) m_ShaderBlob->Release();
	}

}