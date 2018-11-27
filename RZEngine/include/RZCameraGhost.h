#pragma once

namespace rczEngine
{
	class RZ_EXP CameraGhost : public CameraCmp, public IKeyHandler, public IMouseHandler
	{
	public:
		void InitCameraGhost()
		{
			EventManager* evnt = EventManager::Pointer();

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_W);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_W);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_S);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_S);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_D);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_D);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_A);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_A);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_E);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_E);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_Q);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_Q);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_LMENU);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_LMENU);

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_LCONTROL);
			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_UP, KEY_LCONTROL);

			evnt->SubscribeToEvent((IMouseHandler*)this, EVENT_MOUSE_MOVED, -1);

			m_EventMng = evnt;
		};

		virtual void Update(float deltaTime)
		{
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

		/////KEY INTERFACE
		virtual void OnKeyDown(int32 k)
		{
			switch (k)
			{
			case KEY_W:
				m_zAxis = 1.0f;
				break;
			case KEY_A:
				m_xAxis = -1.0f;
				break;
			case KEY_D:
				m_xAxis = 1.0f;
				break;
			case KEY_S:
				m_zAxis = -1.0f;
				break;
			case KEY_E:
				m_yAxis = 1.0f;
				break;
			case KEY_Q:
				m_yAxis = -1.0f;
				break;
			case KEY_LMENU:
				m_Orbiting = true;
				break;
			case KEY_LCONTROL:
				m_Rotating = true;
				break;
			}
		};
		virtual void OnKeyPressed(int32 k) {};
		virtual void OnKeyUp(int32 k)
		{
			switch (k)
			{
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
			case KEY_E:
				m_yAxis = 0.0f;
				break;
			case KEY_Q:
				m_yAxis = 0.0f;
				break;
			case KEY_LMENU:
				m_Orbiting = false;
				break;
			case KEY_LCONTROL:
				m_Rotating = false;
				break;
			}
		};

		virtual void OnClick(int32 mb = 0, int32 mx = 0, int32 my = 0) {};
		virtual void OnHold(int32 mb = 0, int32 mx = 0, int32 my = 0) {};
		virtual void OnReleased(int32 mb = 0, int32 mx = 0, int32 my = 0) {};
		virtual void OnMoved(int32 mb = 0, int32 mx = 0, int32 my = 0) {};

	private:
		float m_xAxis = 0.0f;
		float m_yAxis = 0.0f;
		float m_zAxis = 0.0f;

		float m_SpinY = 0.0f;
		float m_SpinX = 0.0f;

		bool m_Orbiting = false;
		bool m_Rotating = false;

		EventManager* m_EventMng = NULL;
	};
}