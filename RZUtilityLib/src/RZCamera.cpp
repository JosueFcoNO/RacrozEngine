#include <RZUtilityPCH.h>

namespace rczEngine
{
	void Camera::Init(Vector3 position, Vector3 target, float nearClip, float farClip, float aspectRatio)
	{
		m_Position = position;
		m_Target = target;
		m_Up = Vector3(0, 1, 0);
		m_NearClip = nearClip;
		m_FarClip = farClip;
		m_AspectRatio = aspectRatio;
		m_Fov = 45.0f;
	}

	void Camera::Move(Vector3 panning)
	{
		m_Position += panning;
		m_Target += panning;

		m_CachedViewMatrix = false;
	}

	void Camera::MoveForward(float delta)
	{
		Vector3 fwrd = m_Target - m_Position;
		fwrd.Normalize();
		m_Position += fwrd*delta;
		m_Target += fwrd*delta;

		m_CachedViewMatrix = false;
	}

	void Camera::MoveRight(float delta)
	{
		Vector3 fwrd = m_Target - m_Position;
		Vector3 right = m_Up^fwrd;
		right.Normalize();
		
		m_Position += right*delta;
		m_Target   += right*delta;

		m_CachedViewMatrix = false;
	}

	void Camera::MoveUp(float delta)
	{
		m_Position += m_Up*delta;
		m_Target += m_Up*delta;

		m_CachedViewMatrix = false;
	}

	void Camera::Move(float x, float y, float z)
	{
		Vector3 temp(x,y,z);
		m_Position += temp;
		m_Target += temp;

		m_CachedViewMatrix = false;
	}

	void Camera::Rotate(Vector3 vector)
	{
		Vector3 NewVec = (m_Target - m_Position).GetNormalized();
		Vector3 NewVec2 = m_Up.GetNormalized();
		Vector3 NewVec3 = (NewVec^NewVec2).GetNormalized();

		vector /= 100;

		Quaternion AVerQue2(NewVec2, vector.m_y);
		Quaternion AVerQue3(NewVec3, vector.m_x);

		m_Target -= m_Position;
		AVerQue3.Normalize();
		AVerQue2.Normalize();

		Matrix3 temp = AVerQue3.GetAsMatrix3()*AVerQue2.GetAsMatrix3();
		m_Target = temp*m_Target;

		m_Target += m_Position;

		m_CachedViewMatrix = false;

		CalculateUp();
	}

	void Camera::RotateComplete(Vector3 vector)
	{
		Vector3 NewVec = (m_Target - m_Position).GetNormalized();
		Vector3 NewVec2 = m_Up.GetNormalized();
		Vector3 NewVec3 = (NewVec^NewVec2).GetNormalized();

		vector /= 100;

		Quaternion AVerQue2(NewVec2, vector.m_y);
		Quaternion AVerQue3(NewVec3, vector.m_x);

		m_Target -= m_Position;
		AVerQue3.Normalize();
		AVerQue2.Normalize();

		Matrix3 temp = AVerQue3.GetAsMatrix3()*AVerQue2.GetAsMatrix3();
		m_Target = temp*m_Target;
		m_Target += m_Position;

		auto m = Matrix4::LookAtMatrixZ(m_Position, Vector3(0,1,0), m_Target);
		m.m_rows[3].Set(0, 0, 0, 1);

		m_Up = m*Vector3(0,1,0);

		m_CachedViewMatrix = false;
		CalculateUp();
	}

	void Camera::Orbit(Vector3 vector)
	{
		Vector3 NewVec  = (m_Target - m_Position).GetNormalized();
		Vector3 NewVec2 = m_Up.GetNormalized();
		Vector3 NewVec3 = (NewVec^NewVec2).GetNormalized();

		vector /= 100;

		Quaternion AVerQue2(NewVec2, vector.m_y);
		Quaternion AVerQue3(NewVec3, vector.m_x);

		m_Position -= m_Target;

		AVerQue3.Normalize();
		AVerQue2.Normalize();

		Matrix3 temp = AVerQue3.GetAsMatrix3()*AVerQue2.GetAsMatrix3();
		m_Position = temp*m_Position;
		
		m_Position += m_Target;

		m_CachedViewMatrix = false;
		CalculateUp();
	}

	void Camera::Reset(const Vector3 & position, const Vector3 & target, const Vector3 & Up)
	{
		m_Position = position;
		m_Target = target;
		m_Up = Up;

		m_CachedViewMatrix = false;
	}

	const Matrix4 Camera::GetViewMatrix()
	{
		if (!m_CachedViewMatrix)
		{
			m_MatrixView = Matrix4::LookAtMatrix(m_Position, m_Up, m_Target);
			m_CachedViewMatrix = true;


			m_Frustum.CalculateFrustum(*this);
		}

		return m_MatrixView;
	}

	const Matrix4 Camera::GetProjMatrix()
	{
		if (!m_CachedProjectionMatrix)
		{
			m_MatrixProjection = Matrix4::PerpsProjectedSpace(Math::DegreesToRad(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
			m_CachedProjectionMatrix = true;
			m_Frustum.CalculateFrustum(*this);
		}
	
		return m_MatrixProjection;
	}

	void Camera::CalculateUp()
	{
		Vector3 Forward = GetViewDir();
		Vector3 Up = m_Up.GetNormalized();
		Vector3 Right = (Forward^Up).GetNormalized();

		m_Up = Forward ^ Right;
		m_Up.Normalize();

		m_CachedViewMatrix = false;
	}
};