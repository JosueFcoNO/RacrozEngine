#include <RZDirectXPCH.h>

namespace rczEngine
{
	namespace Gfx
	{
		GeometryShader::GeometryShader()
		{
			m_GeometryShader = nullptr;
			m_ShaderBlob = nullptr;
		}

		GeometryShader::~GeometryShader()
		{
			Destroy();
		}

		bool GeometryShader::CreateGeometryShader(const WCHAR * pszFileString, GfxCore * gfx)
		{
			return gfx->CompileAndCreateGeometryShader(*this, pszFileString);
		}

		bool GeometryShader::SetThisGeometryShader(GfxCore * gfx)
		{
			if (m_GeometryShader == nullptr || m_ShaderBlob == nullptr)
				return false;

			return gfx->SetGeometryShader(*this);
		}

		void GeometryShader::Destroy()
		{
			if (m_GeometryShader) m_GeometryShader->Release();
			if (m_ShaderBlob) m_ShaderBlob->Release();
		}
	}
}