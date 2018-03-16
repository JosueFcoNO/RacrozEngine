#include <RZEnginePCH.h>

namespace rczEngine
{
	void Light::InitPointLight(float pointRadius, Vector3 pos, Vector4 color, bool castsShadows)
	{
		m_LightPosition = pos;
		m_LightDirection.Set(0,0,0,0);
		m_LightColor = color;

		m_Props.m_x = (float)LIGHT_POINT;

		if (castsShadows)
		m_Props.m_y = 1;
		else
		m_Props.m_y = 0;

		m_Props.m_z = pointRadius;
		m_Props.m_w = 0.0f;
	}

	void Light::InitSpotLight(float spotFactor, Vector3 pos, Vector3 dir, Vector4 color, bool castsShadows)
	{
		m_LightPosition = pos;
		m_LightDirection = dir;
		m_LightColor = color;

		m_Props.m_x = (float)LIGHT_SPOT;

		if (castsShadows)
			m_Props.m_y = 1;
		else
			m_Props.m_y = 0;

		m_Props.m_z = 0;
		m_Props.m_w = spotFactor;
	}

	void Light::InitDirectionalLight(Vector3 dir, Vector4 color, bool castsShadows)
	{
		m_LightPosition.Set(0,200,0,0);
		m_LightDirection = dir;
		m_LightColor = color;

		m_Props.m_x = (float)LIGHT_DIR;

		if (castsShadows)
			m_Props.m_y = 1;
		else
			m_Props.m_y = 0;

		m_Props.m_z = 0;
		m_Props.m_w = 0;
	}

	Matrix4 Light::GetLightViewProjMatrix()
	{
		switch ((eLIGHT_TYPE)(int)m_Props.m_x)
		{
		default:
		case eLIGHT_TYPE::LIGHT_POINT:
			return Matrix4::LookAtMatrix(m_LightPosition, Vector3(0, 1, 0), m_LightDirection + m_LightPosition) * Matrix4::PerpsProjectedSpace(Math::DegreesToRad(35), 1.0f, 1.0f, 100.0f);
			break;
		case eLIGHT_TYPE::LIGHT_SPOT:
			return Matrix4::LookAtMatrix(m_LightPosition, Vector3(0, 1, 0), m_LightDirection.GetNormalized() + m_LightPosition) * Matrix4::PerpsProjectedSpace(Math::DegreesToRad(90), 1.0f, 1.0f, 100.0f);
			break;
		case eLIGHT_TYPE::LIGHT_DIR:
			Vector4 eye = m_LightDirection;
			eye.Scale(-1);
			return Matrix4::LookAtMatrix(eye, Vector3(0, 1, 0), m_LightDirection);
			break;
		}

	}
}