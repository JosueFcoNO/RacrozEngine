#pragma once

namespace rczEngine
{
	enum class eLightType
	{
		Point,
		Directional,
		Spot
	};

	enum class eLightProps
	{
		Type,
		CastShadows,
		PointRadius,
		SpotFOV
	};
	
	struct LightCore
	{
		Vector4 m_LightColor;	
		Vector4 m_LightPosition;
		Vector4 m_LightDirection;
		Vector4 m_Props;
	};

	///The light class.
	class RZ_EXP Light
	{
	public:
		Light()
		{};

		void InitPointLight(float pointRadius, const Vector3& pos, const Vector4& color, bool castsShadows);
		void InitSpotLight(float spotFactor, const Vector3& pos, const Vector3& dir, const Vector4& color, bool castsShadows);
		void InitDirectionalLight(const Vector3& dir, const Vector4& color, bool castsShadows);

		void InitDepth();

		void SetLightType(eLightType type) 
		{ 
			m_Core.m_Props.m_x = (int)type; 
		};
		
		void SetCastShadows(bool castShadows) 
		{ 
			m_CastShadows = castShadows;

			if (m_CastShadows)
				m_Core.m_Props.m_y = 1;
			else
				m_Core.m_Props.m_y = 0;
		};

		void SetPointRadius(float radius)
		{
			m_Core.m_Props.m_z = radius;
		};

		void SetSpotFOV(float fov)
		{
			m_Core.m_Props.m_w = fov;
		};

		///Update light variables.
		void UpdateLight(const Vector3& pos, const Vector3& direction, const Vector3& color, float intensity);

		///Get the light view projection matrix.
		void GetLightViewProjMatrix(Matrix4& view, Matrix4& proj);

		///The light core.
		LightCore m_Core;

		eLightType m_LightType = eLightType::Point;

		bool m_CastShadows = false;
		Gfx::DepthStencyl m_LightDepth;

	};
};