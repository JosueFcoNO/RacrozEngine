#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Camera
	{
	public:
		Camera() :m_Position(INIT_ZERO), m_Target(INIT_ZERO), m_Up(INIT_ZERO), m_Pitch(INIT_UNIT), yaw(INIT_UNIT), m_Roll(INIT_UNIT)
		{

		};

		void Init(Vector3 position, Vector3 target, float nearClip, float farClip, float aspectRatio);

		void Move(Vector3 panning);
		void MoveForward(float delta);
		void MoveRight(float delta);
		void MoveUp(float delta);
		void Move(float x, float y, float z);
		void Rotate(Vector3 vector);
		void RotateComplete(Vector3 vector);
		void Orbit(Vector3 vector);

		Matrix4 GetViewMatrix();
		Matrix4 GetProjMatrix();
		Vector3 GetOrientation();
		Vector3 GetTarget();
		Vector3 GetPosition();
		void CalculateUp();

		Vector3 m_Position;
		Vector3 m_Target;
		Vector3 m_Up;
		float m_NearClip;
		float m_FarClip;
		float m_AspectRatio;

		Quaternion m_Pitch;
		Quaternion yaw;
		Quaternion m_Roll;
	};
}