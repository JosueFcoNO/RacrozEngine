#include <RZUtilityPCH.h>
#include "RZDirectXPCH.h"

namespace rczEngine
{
	namespace Gfx
	{
		DomainShader::DomainShader()
		{
			m_DomainShader = NULL;
			m_ShaderBlob = NULL;
		}

		DomainShader::~DomainShader()
		{
			Destroy();
		}

		bool DomainShader::CreateDomainShader(const WCHAR * pszFileString, GfxCore * gfx)
		{
			return gfx->CompileAndCreateDomainShader(*this, pszFileString);
		}

		bool DomainShader::SetThisDomainShader(GfxCore * gfx)
		{
			if (m_DomainShader == NULL || m_ShaderBlob == NULL)
				return false;

			return gfx->SetDomainShader(*this);
		}

		void DomainShader::Destroy()
		{
			if (m_DomainShader) m_DomainShader->Release();
			if (m_ShaderBlob) m_ShaderBlob->Release();
		}
	}
};