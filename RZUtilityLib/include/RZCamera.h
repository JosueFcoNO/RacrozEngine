#pragma once

namespace rczEngine
{
	class RZ_EXP Camera
	{
	public:
		Camera() : m_Position(eInit::Zero), m_Target(eInit::Unit), m_Up(eInit::Zero)
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

		const Matrix4 GetViewMatrix();
		const Matrix4 GetProjMatrix();

		FORCEINLINE const Vector3 GetViewDir() const { return (m_Target - m_Position).GetNormalized(); }
		FORCEINLINE const Vector3 GetTarget() const { return m_Target; }
		FORCEINLINE const Vector3 GetPosition() const { return m_Position; }
		FORCEINLINE const Vector3 GetUp() const { return m_Up; }
		FORCEINLINE const Vector3 GetRight() const { return (GetViewDir() ^ m_Up).GetNormalized(); }

		FORCEINLINE void SetNearClip(float nearClip) noexcept { m_NearClip = nearClip; m_CachedProjectionMatrix = false; }
		FORCEINLINE void SetFarClip(float farClip) noexcept { m_FarClip = farClip; m_CachedProjectionMatrix = false; }
		FORCEINLINE void SetAspectRatio(float aspect) noexcept { m_AspectRatio = aspect; m_CachedProjectionMatrix = false; };
		FORCEINLINE void SetFov(float fov) noexcept { m_Fov = fov; m_CachedProjectionMatrix = false; };

		FORCEINLINE float GetNearClip() const noexcept { return m_NearClip; };
		FORCEINLINE float GetFarClip() const noexcept { return m_FarClip; };
		FORCEINLINE float GetAspectRatio() const noexcept { return m_AspectRatio; };
		FORCEINLINE float GetFov() const noexcept { return m_Fov; };

		void CalculateUp();

		Vector3 m_Position;
		Vector3 m_Target;
		Vector3 m_Up;

		Frustum m_Frustum;

	private:
		bool m_CachedViewMatrix;
		bool m_CachedProjectionMatrix;
		bool m_IsPerspective;

		float m_NearClip;
		float m_FarClip;
		float m_AspectRatio;
		float m_Fov;

		Matrix4 m_MatrixView;
		Matrix4 m_MatrixProjection;

	};
}