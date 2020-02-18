#pragma once

namespace rczEngine
{
	class RZ_EXP CameraWalk : public CameraCmp
	{
	public:

		virtual void Init()
		{
			InitCameraWalk(Vector3(0, 10, 10), Vector3(0, 0, 0), 0.1f, 1000.0f, (float)Gfx::GfxCore::Pointer()->GetWidth() / (float)Gfx::GfxCore::Pointer()->GetHeight());
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

			if (Input::GetKeyHold(KEY_W)) m_zAxis = 1.5f;
			else if (Input::GetKeyHold(KEY_S)) m_zAxis = -1.5f;
			else m_zAxis = 0.0f;

			if (Input::GetKeyHold(KEY_D)) m_xAxis = 1.5f;
			else if (Input::GetKeyHold(KEY_A)) m_xAxis = -1.5f;
			else m_xAxis = 0.0f;

			if (Input::GetKeyDown(KEY_SPACE))
			{
				m_Jumping = true;
				m_ySpeed = 5.0f;
			}

			m_Mouse = Input::GetKeyHold(KEY_LMENU);

			m_CameraCore.MoveRight(-deltaTime*m_xAxis*Speed);

			auto VectorDir = m_CameraCore.GetTarget() - m_CameraCore.GetPosition();
			VectorDir.m_y = 0.0f;
			VectorDir.Normalize();

			m_CameraCore.MoveForward(deltaTime*m_zAxis*Speed);

			if (m_Jumping)
			{
				m_ySpeed -= 9.81f*deltaTime;
				if (m_CameraCore.GetPosition().m_y <= 2.0f)
				{
					m_Jumping = false;
					m_ySpeed = 0.0f;
				}
			}

			auto m = Input::Pointer()->GetMouseData();

			if (m_Mouse)
			{
				//
				m_CameraCore.Rotate(Vector3(m.dy*0.15f, -m.dx*0.15f, 0.0f));
			}

			m_Owner.lock()->SetPosition(m_CameraCore.GetPosition());
		};

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

			ImGui::DragFloat("Speed", &Speed, 0.1f, 0.01f, 100.0f);

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
		float m_zAxis = 0.0f;

		float m_ySpeed = 0.0f;

		float m_SpinY = 0.0f;
		float m_SpinX = 0.0f;

		float Speed = 1.0f;

		bool m_Jumping = false;
		bool m_Mouse = false;
		bool m_Active = true;

	};
}