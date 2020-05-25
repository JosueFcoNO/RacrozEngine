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
	}

	void CameraManager::Destroy()
	{
		m_ActiveCamera = nullptr;
		m_Cameras.clear();
	}

	ComponentId CameraManager::AddCamera(StrPtr<CameraCmp> cameraPtr)
	{
		Pair<ComponentId, StrPtr<CameraCmp>> value;
		value.second = cameraPtr;
		value.first = cameraPtr->GetComponentID();;
		m_Cameras.insert(value);

		m_ActiveCamera = cameraPtr;

		return cameraPtr->GetComponentID();
	}

	void CameraManager::RemoveCamera(ComponentId id)
	{
		if (m_ActiveCamera == (m_Cameras.find(id))->second)
		{
			if (m_Cameras.size() > 1)
			{
				m_ActiveCamera = (m_Cameras.begin()++)->second;
			}
		}

		m_Cameras.erase(id);

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

}