#include <RZEnginePCH.h>

namespace rczEngine
{
	void LightComponent::Update(float deltaTime)
	{
		const auto& position = m_Owner.lock()->GetAccumulatedPosition();
		const auto& rotation = m_Owner.lock()->GetOrientation();

		GraphicDebugger::Pointer()->AddPoint("Light", position, 10.0f, Color(1, 1, 1, 1), 0.1f);
		GraphicDebugger::Pointer()->AddLineList("LightList", { position, position + rotation }, Color(1, 1, 1, 1), 0.1f);

		m_Light->UpdateLight(position, rotation, m_Color, m_Intensity);
	};

	void LightComponent::InitLightInManager(LightManager * lManager, Gfx::GfxCore* gfx, eLightType lightType, Vector3 pos, Vector3 dir, Vector4 color, bool castsShadows, float pointRadius, float spotFactor)
	{
		m_Light = lManager->AddLight();

		switch (lightType)
		{
		default:
		case eLightType::Point:
			m_Light->InitPointLight(pointRadius, pos, color, castsShadows);
			break;
		case eLightType::Spot:
			m_Light->InitSpotLight(spotFactor, pos, dir, color, castsShadows);
			break;
		case eLightType::Directional:
			m_Light->InitDirectionalLight(dir, color, castsShadows);
			break;
		}

		m_LightViewBuffer.CreateConstantBuffer(sizeof(Matrix4::m_elements), Gfx::USAGE_DEFAULT, gfx);

		//auto view = m_Light->GetLightViewProjMatrix();
		//m_LightViewBuffer.UpdateConstantBuffer(&view, gfx);
	}

	void LightComponent::SetLightViewMatrix(Gfx::GfxCore * gfx)
	{
		m_LightViewBuffer.SetBufferInVS(1, gfx);
	}
};
