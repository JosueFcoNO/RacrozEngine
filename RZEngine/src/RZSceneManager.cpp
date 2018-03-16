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

	StrPtr<Scene> SceneManager::CreateScene(const char * name)
	{
		StrPtr<Scene> DefaultScene = std::make_shared<Scene>();
		DefaultScene->InitScene();

		m_SavedScenes.push_back(DefaultScene);
		m_ActiveScene = DefaultScene;

		StrGameObjectPtr defaultCamera = DefaultScene->CreateActor("Camera", NULL).lock();
		defaultCamera->AddComponent<CameraWalk>().lock()->InitCameraWalk(Vector3(0, 0, 5), Vector3(0, 0, 0), 0.1f, 1000.0f, (float)Gfx::GfxCore::Pointer()->GetWidth() / (float)Gfx::GfxCore::Pointer()->GetHeight());
		CameraManager::Pointer()->SetActiveCamera("Camera", Gfx::GfxCore::Pointer());

		DefaultScene->m_RootNode->SetScale(1.0f, 1.0f, 1.0f);
		DefaultScene->m_RootNode->SetPosition(0, 0, 0);
		DefaultScene->m_RootNode->SetOrientation(0, 0, 0);

		StrGameObjectPtr Terraine = DefaultScene->CreateActor("Terrain", NULL, Vector3(-256, -15, -256)).lock();
		Terrain* t = &Terraine->AddComponent<rczEngine::TerrainRenderer>().lock()->m_Terrain;
		t->InitTerrain(Gfx::GfxCore::Pointer(), ResVault::Pointer(), ComputeAPI::Pointer());

		return DefaultScene;
	}

	StrPtr<Scene> SceneManager::GetActiveScene()
	{
		return m_ActiveScene;
	}

};