#include <RZUtilityPCH.h>
#include "RZDirectXPCH.h"

namespace rczEngine
{
	namespace Gfx
	{
		PixelShader::PixelShader()
		{
			m_PixelShader = nullptr;
			m_ShaderBlob = nullptr;
		}

		PixelShader::~PixelShader()
		{
			Destroy();
		}

		bool PixelShader::CreatePixelShader(const WCHAR * pszFileString, GfxCore* gfx)
		{
			return gfx->CompileAndCreatePixelShader(*this, pszFileString);
		}

		void PixelShader::SetThisPixelShader(GfxCore* gfx)
		{
			if (m_PixelShader == nullptr || m_ShaderBlob == nullptr)
				return;

			gfx->SetPixelShader(*this);
		}

		void PixelShader::Destroy()
		{
			if (m_PixelShader) m_PixelShader->Release();
			if (m_ShaderBlob) m_ShaderBlob->Release();
		}
	}

}