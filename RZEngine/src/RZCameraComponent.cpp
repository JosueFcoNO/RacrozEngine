#include <RZEnginePCH.h>

namespace rczEngine
{
	void CameraCmp::CreateCamera()
	{
		auto cameraMng = CameraManager::Pointer();
		cameraMng->AddCamera(shared_from_this());
	}

	void CameraCmp::Destroy()
	{
		auto cameraMng = CameraManager::Pointer();
		cameraMng->RemoveCamera(m_ID);
	}

	void CameraCmp::Serialize()
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

	}

	void CameraCmp::DeSerialize()
	{
		auto ser = Serializer::Pointer();

		ser->ReadData(&m_ID, sizeof(int));

		//Position 
		ser->ReadData(&m_CameraCore.m_Position.m_x, sizeof(float));
		ser->ReadData(&m_CameraCore.m_Position.m_y, sizeof(float));
		ser->ReadData(&m_CameraCore.m_Position.m_z, sizeof(float));

		//Target 
		ser->ReadData(&m_CameraCore.m_Target.m_x, sizeof(float));
		ser->ReadData(&m_CameraCore.m_Target.m_y, sizeof(float));
		ser->ReadData(&m_CameraCore.m_Target.m_z, sizeof(float));

		//Up 
		ser->ReadData(&m_CameraCore.m_Up.m_x, sizeof(float));
		ser->ReadData(&m_CameraCore.m_Up.m_y, sizeof(float));
		ser->ReadData(&m_CameraCore.m_Up.m_z, sizeof(float));

		//NearClip, FarClip, AspectRatio, Fov
		ser->ReadData(&m_CameraCore.m_NearClip, sizeof(float));
		ser->ReadData(&m_CameraCore.m_FarClip, sizeof(float));
		ser->ReadData(&m_CameraCore.m_AspectRatio, sizeof(float));
		ser->ReadData(&m_CameraCore.m_Fov, sizeof(float));
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