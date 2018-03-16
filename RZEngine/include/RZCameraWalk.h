#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT CameraWalk : 
		public CameraCmp, 
		public IKeyHandler, 
		public IMouseHandler
	{
	public:
		void InitCameraWalk(Vector3 position, Vector3 target, float nearPlane, float farPlane, float aspectRatio)
		{
			auto evnt = EventManager::Pointer();

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_W);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_W);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_S);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_S);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_D);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_D);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_A);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_A);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_SPACE);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_LMENU);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_LMENU);

			evnt->SubscribeToEvent((IMouseHandler*)this, EVENT_MOUSE_MOVED, -1);

			CreateCamera();
			m_CameraCore.Init(position, target, nearPlane, farPlane, aspectRatio);

			m_Owner.lock()->SetPosition(position);
		};

		virtual void Update(float deltaTime)
		{
			m_CameraCore.MoveRight(-deltaTime*m_xAxis*2);

			auto VectorDir = m_CameraCore.GetTarget() - m_CameraCore.GetPosition();
			VectorDir.m_y = 0.0f;
			VectorDir.Normalize();

			m_CameraCore.Move(VectorDir.m_x*deltaTime*m_zAxis*2, m_ySpeed*deltaTime, VectorDir.m_z*deltaTime*m_zAxis*2);

			if (m_Jumping)
			{
				m_ySpeed -= 9.81*deltaTime;
				if (m_CameraCore.GetPosition().m_y <= 2.0f)
				{
					m_Jumping = false;
					m_ySpeed = 0.0f;
				}
			}

			auto m = Input::Pointer()->GetMouseData();

			if (m_Mouse)
			{
				m_CameraCore.Rotate(Vector3(0.0f, m.dx*0.15f, 0.0f));
				m_CameraCore.Rotate(Vector3(-m.dy*0.15f, 0.0f, 0.0f));
			}

			m_Owner.lock()->SetPosition(m_CameraCore.GetPosition());
		};

		virtual ComponentType GetComponentType() { return CMP_CAMERA_WALK; };
		virtual ComponentId GetComponentId() { return m_ID; };

		/////KEY INTERFACE
		virtual void OnKeyDown(int32 k)
		{
			switch (k)
			{
			case KEY_LMENU:
				m_Mouse = true;
				break;
			case KEY_W:
				m_zAxis = 1.5f;
				break;
			case KEY_A:
				m_xAxis = -1.5f;
				break;
			case KEY_D:
				m_xAxis = 1.5f;
				break;
			case KEY_S:
				m_zAxis = -1.5f;
				break;
			case KEY_SPACE:
				m_Jumping = true;
				m_ySpeed = 5.0f;
				break;
			}
		};
		virtual void OnKeyPressed(int32 k) {};
		virtual void OnKeyUp(int32 k)
		{
			switch (k)
			{
			case KEY_LMENU:
				m_Mouse = false;
				break;
			case KEY_W:
				m_zAxis = 0.0f;
				break;
			case KEY_A:
				m_xAxis = 0.0f;
				break;
			case KEY_D:
				m_xAxis = 0.0f;
				break;
			case KEY_S:
				m_zAxis = 0.0f;
				break;
			}
		};

		/////MOUSE INTERFACE
		virtual void OnClick(int32 mb, int32 mx, int32 my) {};
		virtual void OnHold(int32 mb, int32 mx, int32 my) {};
		virtual void OnReleased(int32 mb, int32 mx, int32 my) {};
		virtual void OnMoved(int32 mb, int32 mx, int32 my)
		{

		};

	private:
		float m_xAxis = 0.0f;
		float m_zAxis = 0.0f;

		float m_ySpeed = 0.0f;

		float m_SpinY = 0.0f;
		float m_SpinX = 0.0f;

		bool m_Jumping = false;
		bool m_Mouse = false;
	};
}