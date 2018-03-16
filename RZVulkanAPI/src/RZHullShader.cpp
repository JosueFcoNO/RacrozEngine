#include <RZUtilityPCH.h>
#include "RZVulkanPCH.h"

namespace rczEngine
{
	HullShader::HullShader()
	{
		m_HullShader = NULL;
		m_ShaderBlob = NULL;
	}

	HullShader::~HullShader()
	{
		Destroy();
	}

	bool HullShader::CreateHullShader(const WCHAR * pszFileString, GfxCore * gfx)
	{
		return gfx->CompileAndCreateHullShader(*this, pszFileString);
	}

	bool HullShader::SetThisHullShader(GfxCore * gfx)
	{
		if (m_HullShader == NULL || m_ShaderBlob == NULL)
			return false;

		return gfx->SetHullShader(*this);
	}

	void HullShader::Destroy()
	{
		if (m_HullShader) m_HullShader->Release();
		if (m_ShaderBlob) m_ShaderBlob->Release();
	}
};