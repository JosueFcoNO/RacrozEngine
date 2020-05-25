#include <RZEnginePCH.h>

namespace rczEngine
{
	void Light::InitPointLight(float pointRadius, const Vector3 & pos, const Vector4 & color, bool castsShadows)
	{
		m_Core.m_LightPosition = pos;
		m_Core.m_LightDirection.Set(0, 0, 0, 0);
		m_Core.m_LightColor = color;

		m_Core.m_Props.m_x = (float)eLightType::Point;

		if (castsShadows)
			m_Core.m_Props.m_y = 1;
		else
			m_Core.m_Props.m_y = 0;

		m_Core.m_Props.m_z = pointRadius;
		m_Core.m_Props.m_w = 0.0f;
	}

	void Light::InitSpotLight(float spotFactor, const Vector3 & pos, const Vector3 & dir, const Vector4 & color, bool castsShadows)
	{
		m_Core.m_LightPosition = pos;
		m_Core.m_LightDirection = dir;
		m_Core.m_LightColor = color;

		m_Core.m_Props.m_x = (float)eLightType::Spot;

		if (castsShadows)
			m_Core.m_Props.m_y = 1;
		else
			m_Core.m_Props.m_y = 0;

		m_Core.m_Props.m_z = 0;
		m_Core.m_Props.m_w = spotFactor;
	}

	void Light::InitDirectionalLight(const Vector3 & dir, const Vector4 & color, bool castsShadows)
	{
		m_Core.m_LightDirection = dir;
		m_Core.m_LightPosition = dir * -100.0f;
		m_Core.m_LightColor = color;

		m_Core.m_Props.m_x = (float)eLightType::Directional;

		if (castsShadows)
			m_Core.m_Props.m_y = 1;
		else
			m_Core.m_Props.m_y = 0;

		m_Core.m_Props.m_z = 0;
		m_Core.m_Props.m_w = 0;
	}

	void Light::InitDepth()
	{
		Gfx::GfxCore::Pointer()->CreateDepthStencyl(m_LightDepth, 1024, 1024);
	}

	void Light::UpdateLight(const Vector3 & pos, const Vector3 & direction, const Vector3 & color, float intensity)
	{
		m_Core.m_LightPosition = pos;
		m_Core.m_LightDirection = direction;
		m_Core.m_LightColor = color;
		m_Core.m_LightColor.m_w = intensity;
	}

	Matrix4 Light::GetLightViewProjMatrix()
	{
		switch ((eLightType)(int)m_Core.m_Props.m_x)
		{
		default:
		case eLightType::Point:
			return Matrix4::LookAtMatrix(m_Core.m_LightPosition, Vector3(0, 1, 0), m_Core.m_LightDirection + m_Core.m_LightPosition) * Matrix4::PerpsProjectedSpace(Math::DegreesToRad(35), 1.0f, 1.0f, 100.0f);
			break;
		case eLightType::Spot:
			return Matrix4::LookAtMatrix(m_Core.m_LightPosition, Vector3(0, 1, 0), m_Core.m_LightDirection.GetNormalized() + m_Core.m_LightPosition) * Matrix4::PerpsProjectedSpace(Math::DegreesToRad(90), 1.0f, 1.0f, 100.0f);
			break;
		case eLightType::Directional:
			Vector4 eye = m_Core.m_LightDirection;
			eye.Scale(-100);
			return Matrix4::LookAtMatrix(eye, Vector3(0, 1, 0), m_Core.m_LightDirection) * Matrix4::OrthoProjectedSpace(512, 512, 1.0f, 5000.0f);
			break;
		}

	}
}