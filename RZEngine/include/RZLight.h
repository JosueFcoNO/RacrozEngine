#pragma once

namespace rczEngine
{
	enum eLIGHT_TYPE
	{
		LIGHT_POINT,
		LIGHT_DIR,
		LIGHT_SPOT
	};

	enum eLIGHT_PROP
	{
		PROP_TYPE,
		PROP_SHADOWS,
		PROP_POINT_RADIUS,
		PROP_SPOT_RANGE
	};

	class RZ_EXP Light
	{
	public:
		Light()
		{};

		void InitPointLight(float pointRadius, Vector3 pos, Vector4 color, bool castsShadows);
		void InitSpotLight(float spotFactor, Vector3 pos, Vector3 dir, Vector4 color, bool castsShadows);
		void InitDirectionalLight(Vector3 dir, Vector4 color, bool castsShadows);

		Matrix4 GetLightViewProjMatrix();

		Vector4 m_LightColor;
		Vector4 m_LightPosition;
		Vector4 m_LightDirection;
		Vector4 m_Props;
	};
};