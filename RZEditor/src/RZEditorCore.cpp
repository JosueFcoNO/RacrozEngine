#include <RZEditorPCH.h>

namespace rczEngine
{
	void EditorCore::InitEditor(PlatformData* platformData)
	{
#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
		RECT rc;
		GetClientRect(platformData->WindowHandle, &rc);
#endif

		Logger::Start();

		Profiler::Start();

		Gfx::GfxCore::Start();
		Gfx::GfxCore::Pointer()->InitAndStart(platformData, rc.right, rc.bottom, true);

		ResVault::Start();
		ResVault::Pointer()->InitResourceManager();

		Input::Start();
		auto input = Input::Pointer();
		input->hinstance = platformData->HandleInstance;
		input->hwnd = platformData->WindowHandle;
		input->InitInput();
	
		ComputeAPI::Start();
		//ComputeAPI::Pointer()->InitComputeAPI(Gfx::GfxCore::Pointer());

		SoundAPI* SndAPI = new SoundAPI;
		SndAPI->InitSound();

		EventManager::Start();
		EventManager::Pointer()->InitEventManager();

		GUIEditor::Start();
		GUIEditor::Pointer()->InitEditor(platformData);

		GraphicDebugger::Start();
		GraphicDebugger::Pointer()->Init();

		CameraManager::Start();
		CameraManager::Pointer()->Init(Gfx::GfxCore::Pointer());

		LightManager::Start();
		LightManager::Pointer()->InitLightManager();

		ActorComponentFactory::Start();

		SceneManager::Start();
		SceneManager::Pointer()->CreateDefaultScene("Default");

		RacrozRenderer::Start();
		RacrozRenderer::Pointer()->InitRenderer();

		StrPtr<SkyBox> Sky = std::make_shared<SkyBox>();
		Sky->InitSkyBox("CubeMaps/Restaurant.dds", "Env", Gfx::GfxCore::Pointer(), ResVault::Pointer());
		RacrozRenderer::Pointer()->ChangeSkyBox(Sky);

		Time.StartTimer();

		m_gfx = Gfx::GfxCore::Pointer();
		m_renderer = RacrozRenderer::Pointer();
		m_scnManager = SceneManager::Pointer();
		//FbxLoader loader;
		//loader.LoadModel();
	}

	void EditorCore::RunEditor()
	{
		while (!Quit)
		{
			Quit = Input::GetKeyDown(KEY_ESCAPE);
			float deltaTime = (float)Time.GetFrameTime();
			
			Input::Pointer()->UpdateInput();
			m_scnManager->GetActiveScene()->Update(deltaTime);

			m_renderer->Render(m_scnManager->GetActiveScene().get(), GUIEditor::Pointer());

			// Bucle principal de mensajes:
			if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				//if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
				//{
				//
				//}
			}
		}
	}

	void EditorCore::DestroyEditor()
	{
		SceneManager::ShutDown();

		ActorComponentFactory::ShutDown();

		ResVault::ShutDown();

		CameraManager::ShutDown();

		LightManager::ShutDown();

		RacrozRenderer::ShutDown();

		Input::ShutDown();

		EventManager::ShutDown();

		GUIEditor::ShutDown();

		GraphicDebugger::ShutDown();

		Profiler::ShutDown();
		Logger::ShutDown();

		Gfx::GfxCore::ShutDown();
	}
}