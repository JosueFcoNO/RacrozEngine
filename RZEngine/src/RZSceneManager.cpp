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

	StrPtr<Scene> SceneManager::CreateDefaultScene(const String& name)
	{
		StrPtr<Scene> DefaultScene = std::make_shared<Scene>();
		DefaultScene->InitScene(name);

		m_SavedScenes.push_back(DefaultScene);

		m_ActiveScene = DefaultScene;

		StrGameObjectPtr defaultCamera = DefaultScene->CreateActor("Camera", NULL).lock();
		
		m_ActiveScene->CreateComponent(CMP_CAMERA_WALK, defaultCamera->GetID());
		auto camera = defaultCamera->GetComponent<CameraWalk>(CMP_CAMERA_WALK).lock();

		StrGameObjectPtr obj = DefaultScene->CreateActor("obj", NULL).lock();
		m_ActiveScene->CreateComponent(CMP_SKINNED_MODEL_RENDERER, obj->GetID());

		CameraManager::Pointer()->SetActiveCamera(camera->GetComponentID(), Gfx::GfxCore::Pointer());

		DefaultScene->m_RootNode->SetScale(1.0f, 1.0f, 1.0f);
		DefaultScene->m_RootNode->SetPosition(0, 0, 0);
		DefaultScene->m_RootNode->SetOrientation(0, 0, 0);
		
		return DefaultScene;
	}

	StrPtr<Scene> SceneManager::CreateEmptyScene(const String& name)
	{
		StrPtr<Scene> DefaultScene = std::make_shared<Scene>();
		DefaultScene->InitScene(name);

		m_SavedScenes.push_back(DefaultScene);

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

	StrPtr<Scene> SceneManager::LoadScene(const String& filePath)
	{
		Serializer::Start();
		auto ser = Serializer::Pointer();

		m_ActiveScene->ClearScene();

		ser->LoadFile(filePath);

		ser->GetNextObjectSerial();

		ser->GetNextObjectSerial();

		int32 resSize;
		ser->ReadData(&resSize, sizeof(resSize));

		auto res = ResVault::Pointer();
		//res->m_ResourceMap.clear();

		for (int32 i = 0; i < resSize; ++i)
		{
			auto ptr = getNewResource((SERIALS)ser->GetNextObjectSerial());
			//ptr->DeSerialize();
			res->InsertResource(ptr);
		}

		uint32 BiggestID = 0;

		while (ser->GetNextObjectSerial() == SERIAL_ACTOR)
		{
			StrGameObjectPtr gO = std::make_shared<GameObject>();
			gO->DeSerialize();

			
			uint32 currentID = gO->GetID();

			if (currentID > BiggestID)
				BiggestID = currentID;
			m_ActiveScene->m_SceneActorMap[currentID] = gO;
		}

		for (auto it : m_ActiveScene->m_SceneActorMap)
		{
			it.second->GetGraphPointers(0);
		}	

		ser->CloseFile();

		ActorComponentFactory::Pointer()->SetLastID(BiggestID);

		return StrPtr<Scene>();
	}

	void SceneManager::SaveScene(const String& filePath)
	{
		Serializer::Start();
		auto ser = Serializer::Pointer();

		ser->StartFile(filePath);

		ser->SetNextObjectSerial(SERIAL_SCENE);

		ser->SetNextObjectSerial(SERIAL_RESOURCE);

		auto vault = ResVault::Pointer();

		int32 size = vault->m_ResourceMap.size();
		ser->WriteData(&size, sizeof(size));

		for (auto it = vault->m_ResourceMap.begin(); it != vault->m_ResourceMap.end(); ++it)
		{
			//it->second->Serialize();
		}

		for (auto it : m_ActiveScene->m_SceneActorMap)
		{
			it.second->Serialize();
		}

		ser->CloseFile();
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