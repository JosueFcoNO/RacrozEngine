#include <RZEnginePCH.h>

namespace rczEngine
{
	SceneManager*& SceneManager::_Instance()
	{
		static SceneManager* instance = nullptr;
		return instance;
	}

	void SceneManager::Start()
	{
		(_Instance()) = new SceneManager;
	}

	SceneManager* SceneManager::Pointer()
	{
		return _Instance();
	}

	void SceneManager::ShutDown()
	{
		delete _Instance();
	}

	StrPtr<Scene> SceneManager::CreateDefaultScene(const String&& name)
	{
		StrPtr<Scene> DefaultScene = std::make_shared<Scene>();
		DefaultScene->InitScene(std::move(name));

		m_Scenes[name] = (DefaultScene);

		m_ActiveScene = DefaultScene;

		StrGameObjectPtr defaultCamera = DefaultScene->CreateActor("Camera", NULL).lock();
		
		m_ActiveScene->CreateComponent(CMP_CAMERA_WALK, defaultCamera->GetID());
		auto camera = defaultCamera->GetComponent<CameraWalk>(CMP_CAMERA_WALK).lock();

		CameraManager::Pointer()->SetActiveCamera(camera->GetComponentID(), Gfx::GfxCore::Pointer());

		DefaultScene->m_RootNode->SetScale(1.0f, 1.0f, 1.0f);
		DefaultScene->m_RootNode->SetPosition(0, 0, 0);
		DefaultScene->m_RootNode->SetOrientation(0, 0, 0);
		
		return DefaultScene;
	}

	StrPtr<Scene> SceneManager::CreateEmptyScene(const String&& name)
	{
		StrPtr<Scene> DefaultScene = std::make_shared<Scene>();
		DefaultScene->InitScene(std::move(name));
		m_Scenes[name] = DefaultScene;

		StrGameObjectPtr defaultCamera = DefaultScene->CreateActor("DefaultCamera", NULL).lock();
		m_ActiveScene->CreateComponent(CMP_CAMERA_WALK, defaultCamera);
		auto camera = defaultCamera->GetComponent<CameraWalk>(CMP_CAMERA_WALK).lock();

		DefaultScene->m_RootNode->SetScale(1.0f, 1.0f, 1.0f);
		DefaultScene->m_RootNode->SetPosition(0, 0, 0);
		DefaultScene->m_RootNode->SetOrientation(0, 0, 0);

		return DefaultScene;
	}

	StrPtr<Scene> SceneManager::GetActiveScene()
	{
		return m_ActiveScene;
	}

	WeakPtr<Scene> SceneManager::GetScene(String sceneName)
	{
		return m_Scenes[sceneName];
	}

	StrPtr<Resource> SceneManager::getNewResource(SERIALS serials)
	{
		switch (serials)
		{
		case rczEngine::SERIAL_MODEL:
			return std::make_shared<Model>();
			break;
		case rczEngine::SERIAL_SKINNED_MODEL:
			return std::make_shared<SkinnedModel>();
			break;
		case rczEngine::SERIAL_TEXTURE2D:
			return std::make_shared<Texture2D>();
			break;
		case rczEngine::SERIAL_MATERIAL:
			return std::make_shared<Material>();
			break;
		case rczEngine::SERIAL_CUBEMAP:
			return std::make_shared<CubeMap>();
			break;
		case rczEngine::SERIAL_ANIMATION:
			return std::make_shared<Animation>();
			break;
		default:
			break;
		}
		return StrPtr<Resource>();
	}

};