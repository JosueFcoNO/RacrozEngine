#pragma once

namespace rczEngine
{
	class RZ_EXP Camera
	{
	public:
		Camera() noexcept :
			m_Position(eInit::Zero),
			m_Target(0, 0, 5),
			m_Up(0, 1, 0),

			m_NearClip(1.0f),
			m_FarClip(100.0f),
			m_AspectRatio(1.0f),
			m_Fov(45.0f),

			m_MatrixView(eInit::Zero),
			m_MatrixProjection(eInit::Zero),

			m_CachedProjectionMatrix(false),
			m_CachedViewMatrix(false),
			m_IsPerspective(true)
		{};

		void Init(const Vector3& position, const Vector3& target, float nearClip, float farClip, float aspectRatio) noexcept;

		void Move(const Vector3& panning) noexcept;
		void MoveForward(float delta) noexcept;
		void MoveRight(float delta) noexcept;
		void MoveUp(float delta) noexcept;
		void Move(float x, float y, float z) noexcept;
		void Rotate(const Vector3& vector) noexcept;
		void RotateComplete(const Vector3& vector) noexcept;
		void Orbit(const Vector3& vector) noexcept;

		void Reset(const Vector3& position, const Vector3& target, const Vector3& Up) noexcept;

		const Matrix4 GetViewMatrix() noexcept;
		const Matrix4 GetProjMatrix() noexcept;

		FORCEINLINE const void SetPosition(const Vector3& position) noexcept { m_Position = position; m_CachedViewMatrix = false; };
		FORCEINLINE const void SetTarget(const Vector3& target) noexcept { m_Target = target; m_CachedViewMatrix = false; };
		FORCEINLINE const void SetUp(const Vector3& up) noexcept { m_Up = up; m_CachedViewMatrix = false; };

		FORCEINLINE const Vector3 GetViewDir() const noexcept { return (m_Target - m_Position).GetNormalized(); }
		FORCEINLINE const Vector3 GetForward() const noexcept { return GetViewDir(); }
		FORCEINLINE const Vector3 GetTarget() const noexcept { return m_Target; }
		FORCEINLINE const Vector3 GetPosition() const noexcept { return m_Position; }
		FORCEINLINE const Vector3 GetUp() const noexcept { return m_Up; }
		FORCEINLINE const Vector3 GetRight() const noexcept { return (GetViewDir() ^ m_Up).GetNormalized(); }

		FORCEINLINE void SetNearClip(float nearClip) noexcept { m_NearClip = nearClip; m_CachedProjectionMatrix = false; }
		FORCEINLINE void SetFarClip(float farClip) noexcept { m_FarClip = farClip; m_CachedProjectionMatrix = false; }
		FORCEINLINE void SetAspectRatio(float aspect) noexcept { m_AspectRatio = aspect; m_CachedProjectionMatrix = false; };
		FORCEINLINE void SetFov(float fov) noexcept { m_Fov = fov; m_CachedProjectionMatrix = false; };
		FORCEINLINE void SetPerspective(bool perspective) noexcept { m_IsPerspective = perspective; m_CachedProjectionMatrix = false; };

		FORCEINLINE void SetWidth(float width) noexcept { m_Width = width; m_CachedProjectionMatrix = false; };
		FORCEINLINE void SetHeight(float height) noexcept { m_Height = height; m_CachedProjectionMatrix = false; };


		FORCEINLINE float GetNearClip() const noexcept { return m_NearClip; };
		FORCEINLINE float GetFarClip() const noexcept { return m_FarClip; };
		FORCEINLINE float GetAspectRatio() const noexcept { return m_AspectRatio; };
		FORCEINLINE float GetFov() const noexcept { return m_Fov; };

		FORCEINLINE float GetWidth() const noexcept { return m_Width; };
		FORCEINLINE float GetHeight() const noexcept { return m_Height; };

		FORCEINLINE const Frustum& GetFrustum() const noexcept { return m_Frustum; };

	private:
		void CalculateUp() noexcept;

		bool m_CachedViewMatrix;
		bool m_CachedProjectionMatrix;
		bool m_IsPerspective = true;

		Vector3 m_Position;
		Vector3 m_Target;
		Vector3 m_Up;

		float m_NearClip;
		float m_FarClip;
		float m_AspectRatio;
		float m_Fov;

		float m_Height = 200;
		float m_Width = 200;

		Matrix4 m_MatrixView;
		Matrix4 m_MatrixProjection;

		Frustum m_Frustum;
	};
}