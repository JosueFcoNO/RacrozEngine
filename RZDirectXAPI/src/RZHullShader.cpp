#include <RZUtilityPCH.h>
#include "RZDirectXPCH.h"

namespace rczEngine
{
	namespace Gfx
	{
		HullShader::HullShader()
		{
			m_HullShader = nullptr;
			m_ShaderBlob = nullptr;
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
			if (m_HullShader == nullptr || m_ShaderBlob == nullptr)
				return false;

			return gfx->SetHullShader(*this);
		}

		void HullShader::Destroy()
		{
			if (m_HullShader) m_HullShader->Release();
			if (m_ShaderBlob) m_ShaderBlob->Release();
		}
	}
};