#pragma once


namespace rczEngine
{
	class RZ_EXP LightComponent : public Component
	{
	public:
		LightComponent() {};

		virtual void Init() {};
		virtual void Update(float deltaTime) { if (m_Set) m_Light->m_LightPosition = m_Owner.lock()->GetAccumulatedPosition(); };

		virtual void Serialize()
		{
			Serializer::Pointer()->SetNextObjectSerial(s_ComponentType + SERIAL_COMPONENT_OFFSET);
		};

		virtual void DeSerialize() {};

		void InitLightInManager(LightManager* lManager, Gfx::GfxCore* gfx, eLIGHT_TYPE lightType, Vector3 pos, Vector3 dir, Vector4 color, bool castsShadows = false, float pointRadius = 1.0f, float spotFactor = .25f);
		void UpdateLight(Vector3 pos, Vector3 dir);
		void SetLightViewMatrix(Gfx::GfxCore* gfx);

		static const ComponentType s_ComponentType = CMP_LIGHT;
		virtual ComponentType GetComponentType() { return LightComponent::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		Light* m_Light;

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Light");

			if (ImGui::Button("Directional Light"))
			{
				m_LightType = eLIGHT_TYPE::LIGHT_DIR;
			}

			if (ImGui::Button("Spot Light"))
			{
				m_LightType = eLIGHT_TYPE::LIGHT_SPOT;
			}

			if (ImGui::Button("Point Light"))
			{
				m_LightType = eLIGHT_TYPE::LIGHT_POINT;
			}

			ImGui::DragFloat3("Pos", &m_Pos.m_x, 0.1f);
			ImGui::DragFloat3("Dir", &m_Direction.m_x, 0.1f);
			//ImGui::ColorPicker4("Color", &m_Color.m_x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float | ImGuiColorEditFlags_::ImGuiColorEditFlags_HDR);

			if (ImGui::Button("SetLight"))
			{
				m_Set = true;
				InitLightInManager(LightManager::Pointer(), Gfx::GfxCore::Pointer(), m_LightType, m_Pos, m_Direction, m_Color);
			}
		}
#endif

	private:
		Gfx::ConstantBuffer m_LightViewBuffer;
		bool m_Set = false;
		eLIGHT_TYPE m_LightType;
		Vector3 m_Pos;
		Vector3 m_Direction;
		Vector4 m_Color;
	};
};