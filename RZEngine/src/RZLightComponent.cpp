#include <RZEnginePCH.h>

namespace rczEngine
{
	void LightComponent::InitLightInManager(LightManager * lManager, Gfx::GfxCore* gfx, eLIGHT_TYPE lightType, Vector3 pos, Vector3 dir, Vector4 color, bool castsShadows, float pointRadius, float spotFactor)
	{
		m_Light = lManager->AddLight();

		switch (lightType)
		{
		default:
		case LIGHT_POINT:
			m_Light->InitPointLight(pointRadius, pos, color, castsShadows);
			break;
		case LIGHT_SPOT:
			m_Light->InitSpotLight(spotFactor, pos, dir, color, castsShadows);
			break;
		case LIGHT_DIR:
			m_Light->InitDirectionalLight(dir, color, castsShadows);
			break;
		}

		m_LightViewBuffer.CreateConstantBuffer(sizeof(Matrix4::m_elements), Gfx::USAGE_DEFAULT, gfx);
		m_LightViewBuffer.UpdateConstantBuffer(&m_Light->GetLightViewProjMatrix(), gfx);
	}

	void LightComponent::UpdateLight(Vector3 pos, Vector3 dir)
	{
		m_Light->m_LightPosition = Matrix4::Translate3D(pos.m_x, pos.m_y, pos.m_z)*m_Light->m_LightPosition;
		m_Light->m_LightDirection = Matrix4::Rotate3D(dir.m_x, dir.m_y, dir.m_z)*m_Light->m_LightDirection;
	}

	void LightComponent::SetLightViewMatrix(Gfx::GfxCore * gfx)
	{
		m_LightViewBuffer.SetBufferInVS(1, gfx);
	}
};