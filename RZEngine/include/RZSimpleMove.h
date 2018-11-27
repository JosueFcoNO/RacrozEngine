#pragma once

namespace rczEngine
{
	class RZ_EXP SimpleMove : public Component
	{
	public:
		SimpleMove() {};

		void Init() {
		};
		void Update(float deltaTime)
		{
			m_Owner.lock()->Rotate(Vector3(0, m_Speed * deltaTime, 0));
		};

		virtual void Serialize() 
		{
			Serializer::Pointer()->SetNextObjectSerial(s_ComponentType + SERIAL_COMPONENT_OFFSET);
		};

		virtual void DeSerialize() {};

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Model Rotator");


			ImGui::DragFloat("Speed", &m_Speed, 0.1f, 0.0f, 100.0f);
		}
#endif

		float m_Speed = 5.0f;

		static const ComponentType s_ComponentType = CMP_MOVE;
		virtual ComponentType GetComponentType() { return SimpleMove::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };
	};
}