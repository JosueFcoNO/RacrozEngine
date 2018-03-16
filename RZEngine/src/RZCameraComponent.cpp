#include <RZEnginePCH.h>

namespace rczEngine
{
	void CameraCmp::CreateCamera()
	{
		auto cameraMng = CameraManager::Pointer();
		cameraMng->AddCamera(shared_from_this());
	}

	Matrix4 CameraCmp::GetViewMatrix()
	{
		return m_CameraCore.GetViewMatrix();
	}

	Matrix4 CameraCmp::GetProjMatrix()
	{
		return m_CameraCore.GetProjMatrix();
	}

	Vector3 CameraCmp::GetOrientation()
	{
		return (m_CameraCore.m_Target - m_CameraCore.m_Position).GetNormalized();
	}

	Vector3 CameraCmp::GetPosition()
	{
		return m_CameraCore.m_Position;
	}

	float CameraCmp::GetNearPlane()
	{
		return m_CameraCore.m_NearClip;
	}

	float CameraCmp::GetFarPlane()
	{
		return m_CameraCore.m_FarClip;
	}
};