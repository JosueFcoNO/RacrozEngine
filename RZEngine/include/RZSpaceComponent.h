#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT SpaceComponent : public Component
	{
	public:
		SpaceComponent() {};

		virtual void Serialize()
		{
			Serializer::Pointer()->SetNextObjectSerial(s_ComponentType + SERIAL_COMPONENT_OFFSET);
		};

		virtual void DeSerialize() {};

		virtual void Init()
		{
			m_Space.InitSpaceManager(GetOwner().lock().get());
		};
		virtual void Update(float deltaTime) { m_Space.Update(deltaTime); };
		virtual void Render(Gfx::GfxCore* gfx, ResVault* res, Scene* scene)
		{
			m_Space.Render();
		};

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Space Manager");

			static bool showScale = false;

			if (ImGui::Button("Show")) showScale = true;

			//Vector3 Scale;
			//
			//Scale = m_Space.m_CurrentPlanet->m_HeightScale;
			//
			//if (showScale && m_Space.m_CurrentPlanet)
			//{
			//	ImGui::DragFloat("Scale", &Scale.m_x, 0.001f, 0.0f, 30.0f);
			//	ImGui::DragFloat("Octaves", &Scale.m_y, 1.0f, 0.0f, 30.0f);
			//	ImGui::DragFloat("Amplitude", &Scale.m_z, 0.1f, 0.0f, 30.0f);
			//
			//}
			//
			//m_Space.m_CurrentPlanet->m_HeightScale = Scale;
		}
#endif

		static const ComponentType s_ComponentType = CMP_SPACE_MANAGER;
		virtual ComponentType GetComponentType() { return SpaceComponent::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		SpaceManager m_Space;
	};
}
