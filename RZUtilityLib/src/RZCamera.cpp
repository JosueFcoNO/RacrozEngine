#include <RZUtilityPCH.h>

namespace rczEngine
{
	void Camera::Init(const Vector3& position, const Vector3& target, float nearClip, float farClip, float aspectRatio) noexcept
	{
		m_Position = position;
		m_Target = target;
		m_Up = Vector3(0, 1, 0);
		m_NearClip = nearClip;
		m_FarClip = farClip;
		m_AspectRatio = aspectRatio;
		m_Fov = 45.0f;
	}

	void Camera::Move(const Vector3& panning) noexcept
	{
		m_Position += panning;
		m_Target += panning;

		m_CachedViewMatrix = false;
	}

	void Camera::MoveForward(float delta) noexcept
	{
		Vector3 fwrd = m_Target - m_Position;
		fwrd.Normalize();
		m_Position += fwrd*delta;
		m_Target += fwrd*delta;

		m_CachedViewMatrix = false;
	}

	void Camera::MoveRight(float delta) noexcept
	{
		const Vector3 fwrd = m_Target - m_Position;
		Vector3 right = m_Up^fwrd;
		right.Normalize();
		
		m_Position += right*delta;
		m_Target   += right*delta;

		m_CachedViewMatrix = false;
	}

	void Camera::MoveUp(float delta) noexcept
	{
		m_Position += m_Up*delta;
		m_Target += m_Up*delta;

		m_CachedViewMatrix = false;
	}

	void Camera::Move(float x, float y, float z) noexcept
	{
		const Vector3 temp(x,y,z);

		m_Position += temp;
		m_Target += temp;

		m_CachedViewMatrix = false;
	}

	void Camera::Rotate(const Vector3& vector)
	{
		const Vector3 NewVec = (m_Target - m_Position).GetNormalized();
		const Vector3 NewVec2 = m_Up.GetNormalized();
		const Vector3 NewVec3 = (NewVec^NewVec2).GetNormalized();

		auto v = vector;
		v /= 100;

		Quaternion AVerQue2(NewVec2, v.m_y);
		Quaternion AVerQue3(NewVec3, v.m_x);

		m_Target -= m_Position;
		AVerQue3.Normalize();
		AVerQue2.Normalize();

		const Matrix3 temp = AVerQue3.GetAsMatrix3()*AVerQue2.GetAsMatrix3();
		m_Target = temp*m_Target;

		m_Target += m_Position;

		m_CachedViewMatrix = false;

		CalculateUp();
	}

	void Camera::RotateComplete(const Vector3& vector)
	{
		const Vector3 NewVec = (m_Target - m_Position).GetNormalized();
		const Vector3 NewVec2 = m_Up.GetNormalized();
		const Vector3 NewVec3 = (NewVec^NewVec2).GetNormalized();

		auto v = vector;
		v /= 100;

		Quaternion AVerQue2(NewVec2, v.m_y);
		Quaternion AVerQue3(NewVec3, v.m_x);

		m_Target -= m_Position;
		AVerQue3.Normalize();
		AVerQue2.Normalize();

		const Matrix3 temp = AVerQue3.GetAsMatrix3()*AVerQue2.GetAsMatrix3();
		m_Target = temp*m_Target;
		m_Target += m_Position;

		auto m = Matrix4::LookAtMatrixZ(m_Position, Vector3(0,1,0), m_Target);
		m.m_rows[3].Set(0, 0, 0, 1);

		m_Up = m*Vector3(0,1,0);

		m_CachedViewMatrix = false;
		CalculateUp();
	}

	void Camera::Orbit(const Vector3& vector)
	{
		const Vector3 NewVec  = (m_Target - m_Position).GetNormalized();
		const Vector3 NewVec2 = m_Up.GetNormalized();
		const Vector3 NewVec3 = (NewVec^NewVec2).GetNormalized();

		auto v = vector;
		v /= 100;

		Quaternion AVerQue2(NewVec2, v.m_y);
		Quaternion AVerQue3(NewVec3, v.m_x);

		m_Position -= m_Target;

		AVerQue3.Normalize();
		AVerQue2.Normalize();

		const Matrix3 temp = AVerQue3.GetAsMatrix3()*AVerQue2.GetAsMatrix3();
		m_Position = temp*m_Position;
		
		m_Position += m_Target;

		m_CachedViewMatrix = false;
		CalculateUp();
	}

	void Camera::Reset(const Vector3 & position, const Vector3 & target, const Vector3 & Up) noexcept
	{
		m_Position = position;
		m_Target = target;
		m_Up = Up;

		m_CachedViewMatrix = false;
		m_CachedProjectionMatrix = false;
	}

	const Matrix4 Camera::GetViewMatrix() noexcept
	{
		if (!m_CachedViewMatrix)
		{
			m_MatrixView = Matrix4::LookAtMatrix(m_Position, m_Up, m_Target);
			m_CachedViewMatrix = true;

			m_Frustum.CalculateFrustum(*this);
		}

		return m_MatrixView;
	}

	const Matrix4 Camera::GetProjMatrix() noexcept
	{
		if (!m_CachedProjectionMatrix)
		{
			m_MatrixProjection = Matrix4::PerpsProjectedSpace(Math::DegreesToRad(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
			m_CachedProjectionMatrix = true;

			m_Frustum.CalculateFrustum(*this);
		}
	
		return m_MatrixProjection;
	}

	void Camera::CalculateUp() noexcept
	{
		const Vector3 Forward = GetViewDir();
		const Vector3 Up = m_Up.GetNormalized();
		const Vector3 Right = (Forward^Up).GetNormalized();

		m_Up = Forward ^ Right;
		m_Up.Normalize();

		m_CachedViewMatrix = false;
	}
};