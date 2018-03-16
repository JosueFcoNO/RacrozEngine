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
		Pair<String, StrPtr<CameraCmp>> value;
		value.second = cameraPtr;
		value.first = value.second->GetOwner().lock()->GetName();
		m_Cameras.insert(value);
	}

	void CameraManager::RemoveCamera(char* name)
	{
		m_Cameras.erase(name);
	}

	WeakPtr<CameraCmp> CameraManager::GetActiveCamera()
	{
		return m_ActiveCamera;
	}

	void CameraManager::SetActiveCamera(char* name, Gfx::GfxCore* gfx)
	{
		auto i = m_Cameras.find(name);
		if (i != m_Cameras.end())
		{
			m_ActiveCamera = i->second;
		}
	}

	void CameraManager::UpdateAndSetCameraBuffer(Gfx::GfxCore * gfx, int32 vertexShaderSlot, int32 PixelShaderSlot)
	{
		m_ActiveCameraData.FarPlane.m_x = m_ActiveCamera->GetFarPlane();
		m_ActiveCameraData.NearPlane.m_x = m_ActiveCamera->GetNearPlane();
		m_ActiveCameraData.ProjectionMatrix = m_ActiveCamera->GetProjMatrix().GetTransposed();
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