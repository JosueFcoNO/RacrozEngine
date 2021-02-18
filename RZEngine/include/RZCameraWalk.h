#pragma once

namespace rczEngine
{
	class RZ_EXP CameraWalk : public CameraCmp
	{
	public:

		virtual void Init()
		{
			InitCameraWalk(Vector3(-800, 40, -50), Vector3(-800, 140, -50) + Vector3(500, 0, 0), 0.25f, 5000.0f, (float)Gfx::GfxCore::Pointer()->GetWidth() / (float)Gfx::GfxCore::Pointer()->GetHeight());
		}

		void InitCameraWalk(Vector3 position, Vector3 target, float nearPlane, float farPlane, float aspectRatio)
		{
			CreateCamera();
			m_CameraCore.Init(position, target, nearPlane, farPlane, aspectRatio);

			m_Owner.lock()->SetPosition(position);
		};

		virtual void Update(float deltaTime)
		{
			if (!m_Active) return;

			if (ImGui::GetIO().KeysDown['W']) m_zAxis = 1.5f;
			else if (ImGui::GetIO().KeysDown['S']) m_zAxis = -1.5f;
			else m_zAxis = 0.0f;

			if (ImGui::GetIO().KeysDown['D']) m_xAxis = -1.5f;
			else if (ImGui::GetIO().KeysDown['A']) m_xAxis = 1.5f;
			else m_xAxis = 0.0f;

			if (ImGui::GetIO().KeysDown['Q']) m_yAxis = -1.5f;
			else if (ImGui::GetIO().KeysDown['E']) m_yAxis = 1.5f;
			else m_yAxis = 0.0f;

			if (ImGui::IsKeyDown(KEY_SPACE))
			{
				m_Jumping = true;
				m_ySpeed = 5.0f;
			}

			m_Mouse = ImGui::IsMouseDown(ImGuiMouseButton_Middle);

			m_CameraCore.MoveRight(-deltaTime*m_xAxis*Speed);

			auto VectorDir = m_CameraCore.GetTarget() - m_CameraCore.GetPosition();
			VectorDir.m_y = 0.0f;
			VectorDir.Normalize();

			m_CameraCore.MoveForward(deltaTime*m_zAxis*Speed);

			m_CameraCore.MoveUp(deltaTime*m_yAxis*Speed);


			if (m_Jumping)
			{
				m_ySpeed -= 9.81f*deltaTime;
				if (m_CameraCore.GetPosition().m_y <= 2.0f)
				{
					m_Jumping = false;
					m_ySpeed = 0.0f;
				}
			}

			auto m = ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Middle);
			//auto sx = Math::Sign(m.x);
			//auto sy = Math::Sign(m.y);
			//m.x = abs(m.x), 1.0f);
			//m.y = abs(m.y), 1.0f);

			if (m_Mouse)
			{
				m_CameraCore.Rotate(Vector3(m.y* 0.01f, -m.x * 0.01f, 0.0f));
			}

			m_Owner.lock()->SetPosition(m_CameraCore.GetPosition());
		};

		ImVec2 m_LastPos;

		virtual void Serialize()
		{
			auto ser = Serializer::Pointer();
			ser->SetNextObjectSerial(s_ComponentType + SERIAL_COMPONENT_OFFSET);

			ser->WriteData(&m_ID, sizeof(int));

			auto pos = m_CameraCore.GetPosition();

			//Position 
			ser->WriteData(&pos.m_x, sizeof(float));
			ser->WriteData(&pos.m_y, sizeof(float));
			ser->WriteData(&pos.m_z, sizeof(float));

			auto target = m_CameraCore.GetTarget();

			//Target 
			ser->WriteData(&target.m_x, sizeof(float));
			ser->WriteData(&target.m_y, sizeof(float));
			ser->WriteData(&target.m_z, sizeof(float));

			auto up = m_CameraCore.GetUp();

			//Up 
			ser->WriteData(&up.m_x, sizeof(float));
			ser->WriteData(&up.m_y, sizeof(float));
			ser->WriteData(&up.m_z, sizeof(float));

			auto Fov = m_CameraCore.GetFov();
			auto AspectRatio = m_CameraCore.GetAspectRatio();
			auto FarClip = m_CameraCore.GetFarClip();
			auto NearClip = m_CameraCore.GetNearClip();

			//NearClip, FarClip, AspectRatio, Fov
			ser->WriteData(&NearClip, sizeof(float));
			ser->WriteData(&FarClip, sizeof(float));
			ser->WriteData(&AspectRatio, sizeof(float));
			ser->WriteData(&Fov, sizeof(float));

		};

		static const ComponentType s_ComponentType = CMP_CAMERA_WALK;
		virtual ComponentType GetComponentType() { return CMP_CAMERA_WALK; };
		virtual ComponentId GetComponentId() { return m_ID; };

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			CameraCmp::RenderComponent();

			ImGui::DragFloat("Speed", &Speed, 0.1f, 0.01f, 1000.0f);

			if (ImGui::Button("Set Active"))
			{
				CameraManager::Pointer()->SetActiveCamera(m_ID, Gfx::GfxCore::Pointer());

				m_Active = true;
			}

			if (ImGui::Button("Deactivate")) m_Active = false;
		}
#endif

	private:
		float m_xAxis = 0.0f;
		float m_yAxis = 0.0f;
		float m_zAxis = 0.0f;

		float m_ySpeed = 0.0f;

		float m_SpinY = 0.0f;
		float m_SpinX = 0.0f;

		float Speed = 150.0f;

		bool m_Jumping = false;
		bool m_Mouse = false;
		bool m_Active = true;

	};
}