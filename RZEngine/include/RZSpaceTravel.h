#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT SpaceTravel : public Component, public IKeyHandler, public IMouseHandler
	{
	public:
		void InitSpaceTravel(Camera* camera, ResVault* res, EventManager* evnt)
		{
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

			m_Camera = camera;

			m_EventMng = evnt;
		};

		virtual void Update(float deltaTime)
		{
			float speed = 10.0f;

			auto m = Input::Pointer()->GetMouseData();

			if (m_Rotating)
			{
				m_Camera->Rotate(Vector3(0.0f, m.dx*0.25f, 0.0f));
				m_Camera->Rotate(Vector3(-m.dy*0.25f, 0.0f, 0.0f));
			}

			m_Owner.lock()->Translate(m_Camera->GetOrientation() * speed * m_zAxis * deltaTime);

			auto Offset = m_Camera->GetOrientation();
			m_Camera->m_Position = m_Owner.lock()->GetAccumulatedPosition();
			m_Camera->m_Target = m_Camera->m_Position + Offset;
		};

		virtual ComponentType GetComponentType() { return CMP_SPACE_TRAVEL; };
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

		Camera* m_Camera = NULL;

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