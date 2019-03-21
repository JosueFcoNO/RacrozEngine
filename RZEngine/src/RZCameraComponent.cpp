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

		auto pos = m_CameraCore.GetPosition();
		auto target = m_CameraCore.GetTarget();
		auto up = m_CameraCore.GetUp();

		//Position 
		ser->WriteData(&pos.m_x, sizeof(float));
		ser->WriteData(&pos.m_y, sizeof(float));
		ser->WriteData(&pos.m_z, sizeof(float));


		//Target 
		ser->WriteData(&target.m_x, sizeof(float));
		ser->WriteData(&target.m_y, sizeof(float));
		ser->WriteData(&target.m_z, sizeof(float));


		//Up 
		ser->WriteData(&up.m_x, sizeof(float));
		ser->WriteData(&up.m_y, sizeof(float));
		ser->WriteData(&up.m_z, sizeof(float));

		auto nearClip = m_CameraCore.GetNearClip();
		auto farClip = m_CameraCore.GetFarClip();
		auto aspectRatio = m_CameraCore.GetAspectRatio();
		auto fov = m_CameraCore.GetFov();

		//NearClip, FarClip, AspectRatio, Fov
		ser->WriteData(&nearClip, sizeof(float));
		ser->WriteData(&farClip, sizeof(float));
		ser->WriteData(&aspectRatio, sizeof(float));
		ser->WriteData(&fov, sizeof(float));

	}

	void CameraCmp::DeSerialize()
	{
		auto ser = Serializer::Pointer();

		ser->ReadData(&m_ID, sizeof(int));

		auto pos = m_CameraCore.GetPosition();
		auto target = m_CameraCore.GetTarget();
		auto up = m_CameraCore.GetUp();

		//Position 
		ser->ReadData(&pos.m_x, sizeof(float));
		ser->ReadData(&pos.m_y, sizeof(float));
		ser->ReadData(&pos.m_z, sizeof(float));

		//Target 
		ser->ReadData(&target.m_x, sizeof(float));
		ser->ReadData(&target.m_y, sizeof(float));
		ser->ReadData(&target.m_z, sizeof(float));

		//Up 
		ser->ReadData(&up.m_x, sizeof(float));
		ser->ReadData(&up.m_y, sizeof(float));
		ser->ReadData(&up.m_z, sizeof(float));

		m_CameraCore.Reset(pos, target, up);

		auto nearClip = m_CameraCore.GetNearClip();
		auto farClip = m_CameraCore.GetFarClip();
		auto aspectRatio = m_CameraCore.GetAspectRatio();
		auto fov = m_CameraCore.GetFov();

		//NearClip, FarClip, AspectRatio, Fov
		ser->ReadData(&nearClip, sizeof(float));
		ser->ReadData(&farClip, sizeof(float));
		ser->ReadData(&aspectRatio, sizeof(float));
		ser->ReadData(&fov, sizeof(float));

		m_CameraCore.SetNearClip(nearClip);
		m_CameraCore.SetFarClip(farClip);
		m_CameraCore.SetAspectRatio(aspectRatio);
		m_CameraCore.SetFov(fov);
	}

	Matrix4 CameraCmp::GetViewMatrix()
	{
		return m_CameraCore.GetViewMatrix();
	}

	Matrix4 CameraCmp::GetProjMatrix()
	{
		return m_CameraCore.GetProjMatrix();
	}

	Vector3 CameraCmp::GetViewDir()
	{
		return m_CameraCore.GetViewDir().GetNormalized();
	}

	Vector3 CameraCmp::GetPosition()
	{
		return m_CameraCore.GetPosition();
	}

	float CameraCmp::GetNearPlane()
	{
		return m_CameraCore.GetNearClip();
	}

	float CameraCmp::GetFarPlane()
	{
		return m_CameraCore.GetFarClip();
	}
};