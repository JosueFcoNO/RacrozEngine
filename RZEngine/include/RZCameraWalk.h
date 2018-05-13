#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT CameraWalk : 
		public CameraCmp, 
		public IKeyHandler, 
		public IMouseHandler
	{
	public:

		virtual void Init()
		{
			InitCameraWalk(Vector3(0, 0, 5), Vector3(0, 0, 0), 0.1f, 1000.0f, (float)Gfx::GfxCore::Pointer()->GetWidth() / (float)Gfx::GfxCore::Pointer()->GetHeight());
		}

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
			m_CameraCore.MoveRight(-deltaTime*m_xAxis*30);

			auto VectorDir = m_CameraCore.GetTarget() - m_CameraCore.GetPosition();
			VectorDir.m_y = 0.0f;
			VectorDir.Normalize();

			m_CameraCore.MoveForward(deltaTime*m_zAxis*1);

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

		virtual void Serialize()
		{
			auto ser = Serializer::Pointer();
			ser->SetNextObjectSerial(s_ComponentType + SERIAL_COMPONENT_OFFSET);

			ser->WriteData(&m_ID, sizeof(int));

			//Position 
			ser->WriteData(&m_CameraCore.m_Position.m_x, sizeof(float));
			ser->WriteData(&m_CameraCore.m_Position.m_y, sizeof(float));
			ser->WriteData(&m_CameraCore.m_Position.m_z, sizeof(float));

			//Target 
			ser->WriteData(&m_CameraCore.m_Target.m_x, sizeof(float));
			ser->WriteData(&m_CameraCore.m_Target.m_y, sizeof(float));
			ser->WriteData(&m_CameraCore.m_Target.m_z, sizeof(float));

			//Up 
			ser->WriteData(&m_CameraCore.m_Up.m_x, sizeof(float));
			ser->WriteData(&m_CameraCore.m_Up.m_y, sizeof(float));
			ser->WriteData(&m_CameraCore.m_Up.m_z, sizeof(float));

			//NearClip, FarClip, AspectRatio, Fov
			ser->WriteData(&m_CameraCore.m_NearClip, sizeof(float));
			ser->WriteData(&m_CameraCore.m_FarClip, sizeof(float));
			ser->WriteData(&m_CameraCore.m_AspectRatio, sizeof(float));
			ser->WriteData(&m_CameraCore.m_Fov, sizeof(float));

		};

		static const ComponentType s_ComponentType = CMP_CAMERA_WALK;
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