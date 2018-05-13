#include <RZEnginePCH.h>

namespace rczEngine
{
	CameraManager*& CameraManager::_Instance()
	{
		static CameraManager* instance = nullptr;
		return instance;
	}

	void CameraManager::Start()
	{
		(_Instance()) = new CameraManager;
	}

	CameraManager* CameraManager::Pointer()
	{
		return _Instance();
	}

	void CameraManager::ShutDown()
	{
		delete _Instance();
	}

	void CameraManager::Init(Gfx::GfxCore* gfx)
	{
		m_CameraBuffer.CreateConstantBuffer(sizeof(CameraData), Gfx::USAGE_DEFAULT, gfx);
	}

	void CameraManager::Destroy()
	{
		m_CameraBuffer.Destroy();
		m_ActiveCamera = NULL;
		m_Cameras.clear();
	}

	void CameraManager::AddCamera(StrPtr<CameraCmp> cameraPtr)
	{
		Pair<ComponentId, StrPtr<CameraCmp>> value;
		value.second = cameraPtr;
		value.first = cameraPtr->m_ID;
		m_Cameras.insert(value);

		m_ActiveCamera = cameraPtr;
	}

	void CameraManager::RemoveCamera(ComponentId id)
	{
		m_Cameras.erase(id);

		if (m_ActiveCamera->m_ID == id)
		{
			m_ActiveCamera = m_Cameras.begin()->second;
		}
	}

	WeakPtr<CameraCmp> CameraManager::GetActiveCamera()
	{
		return m_ActiveCamera;
	}

	void CameraManager::SetActiveCamera(ComponentId id, Gfx::GfxCore * gfx)
	{
		auto it = m_Cameras.find(id);
		if (it != m_Cameras.end())
		{
			m_ActiveCamera = it->second;
		}
	}

	void CameraManager::UpdateAndSetCameraBuffer(Gfx::GfxCore * gfx, int32 vertexShaderSlot, int32 PixelShaderSlot)
	{
		if (m_ActiveCamera)
		{
			m_ActiveCameraData.FarPlane.m_x = m_ActiveCamera->GetFarPlane();
			m_ActiveCameraData.NearPlane.m_x = m_ActiveCamera->GetNearPlane();

			m_ActiveCameraData.PreviousProjectionMatrix = m_ActiveCameraData.ProjectionMatrix;
			m_ActiveCameraData.ProjectionMatrix = m_ActiveCamera->GetProjMatrix().GetTransposed();

			m_ActiveCameraData.PreviousViewMatrix = m_ActiveCameraData.ViewMatrix;
			m_ActiveCameraData.ViewMatrix = m_ActiveCamera->GetViewMatrix().GetTransposed();

			m_ActiveCameraData.ViewPosition = m_ActiveCamera->GetPosition();
			m_ActiveCameraData.ViewDirection = m_ActiveCamera->GetOrientation();


			m_CameraBuffer.UpdateConstantBuffer(&m_ActiveCameraData, gfx);
			m_CameraBuffer.SetBufferInPS(PixelShaderSlot, gfx);
			m_CameraBuffer.SetBufferInVS(vertexShaderSlot, gfx);
			m_CameraBuffer.SetBufferInDS(vertexShaderSlot, gfx);
			m_CameraBuffer.SetBufferInHS(vertexShaderSlot, gfx);
		}

	}
	
}