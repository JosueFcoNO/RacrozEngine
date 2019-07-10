#pragma once

namespace rczEngine
{
	class RZ_EXP CameraGhost : public CameraCmp
	{
	public:
		void InitCameraGhost()
		{
		};

		virtual void Update(float deltaTime)
		{
			if (Input::GetKeyHold(KEY_W)) m_zAxis = 1.5f;
			else if (Input::GetKeyHold(KEY_S)) m_zAxis = -1.5f;
			else m_zAxis = 0.0f;

			if (Input::GetKeyHold(KEY_D)) m_xAxis = 1.5f;
			else if (Input::GetKeyHold(KEY_A)) m_xAxis = -1.5f;
			else m_xAxis = 0.0f;

			m_Orbiting = Input::GetKeyHold(KEY_LMENU);
			m_Rotating = Input::GetKeyHold(KEY_LCONTROL);

			auto speed = 15;

			m_CameraCore.MoveRight(deltaTime*-m_xAxis*speed);
			m_CameraCore.MoveForward(deltaTime*m_zAxis*speed);
			m_CameraCore.MoveUp(deltaTime*m_yAxis*speed);

			auto m = Input::Pointer()->GetMouseData();

			if (m_Orbiting)
			{
				m_CameraCore.Orbit(Vector3(0.0f, -m.dx * 0.25f, 0.0f));
				m_CameraCore.Orbit(Vector3(m.dy*0.25f, 0.0f, 0.0f));
			}
			else if (m_Rotating)
			{
				m_CameraCore.Rotate(Vector3(0.0f, m.dx*0.25f, 0.0f));
				m_CameraCore.Rotate(Vector3(-m.dy*0.25f, 0.0f, 0.0f));
			}

			m_Owner.lock()->SetPosition(m_CameraCore.GetPosition());

		};

		virtual ComponentType GetComponentType() { return CMP_CAMERA_GHOST; };
		virtual ComponentId GetComponentId() { return m_ID; };

	private:
		float m_xAxis = 0.0f;
		float m_yAxis = 0.0f;
		float m_zAxis = 0.0f;

		float m_SpinY = 0.0f;
		float m_SpinX = 0.0f;

		bool m_Orbiting = false;
		bool m_Rotating = false;
	};
}