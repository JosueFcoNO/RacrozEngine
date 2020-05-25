#pragma once


namespace rczEngine
{
	class RZ_EXP LightComponent : public Component
	{
	public:
		LightComponent() {};

		virtual void Init() 
		{
			m_LightType = eLightType::Directional;
			m_Direction.m_z = -1.0f;
			m_Intensity = 100.0f;
			m_Color = Color(1, 1, 1, 1);
			InitLightInManager(LightManager::Pointer(), Gfx::GfxCore::Pointer(), m_LightType, m_Pos, m_Direction, m_Color, false, pointRadius);

			const auto& position = m_Owner.lock()->GetAccumulatedPosition();
			const auto& rotation = m_Owner.lock()->GetOrientation();

			m_Light->UpdateLight(position, rotation, m_Color, m_Intensity);
		};

		virtual void Update(float deltaTime);

		virtual void Serialize()
		{
			Serializer::Pointer()->SetNextObjectSerial(s_ComponentType + SERIAL_COMPONENT_OFFSET);
		};

		virtual void DeSerialize() {};

		void InitLightInManager(LightManager* lManager, Gfx::GfxCore* gfx, eLightType lightType, Vector3 pos, Vector3 dir, Vector4 color, bool castsShadows = false, float pointRadius = 1.0f, float spotFactor = .25f);
		void SetLightViewMatrix(Gfx::GfxCore* gfx);

		static const ComponentType s_ComponentType = CMP_LIGHT;
		virtual ComponentType GetComponentType() { return LightComponent::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		Light* m_Light;

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();

			if (ImGui::Button("Directional Light"))
			{
				m_LightType = eLightType::Directional;
				m_Light->SetLightType(m_LightType);
			}

			ImGui::SameLine();
			
			if (ImGui::Button("Spot Light"))
			{
				m_LightType = eLightType::Spot;
				m_Light->SetLightType(m_LightType);
			}

			ImGui::SameLine();
			
			if (ImGui::Button("Point Light"))
			{
				m_LightType = eLightType::Point;
				m_Light->SetLightType(m_LightType);
			}
		
			ImGui::ColorPicker4("Color", &m_Color.m_x);

			ImGui::DragFloat("Intensity", &m_Intensity);

			ImGui::Checkbox("Cast Shadows", &m_CastShadows);
			m_Light->SetCastShadows(m_CastShadows);

		}
#endif

	private:
		Gfx::ConstantBuffer m_LightViewBuffer;

		///The light type
		eLightType m_LightType;

		///The position of the light.
		Vector3 m_Pos;

		///The direction of the light
		Vector3 m_Direction;

		///The light intensity
		float m_Intensity = 1.0f;
		
		///The color of the light
		Vector4 m_Color;

		///Cast shadows
		bool m_CastShadows = false;

		float pointRadius = 500.0f;
	};
};