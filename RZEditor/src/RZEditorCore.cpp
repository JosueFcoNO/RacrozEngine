#include <RZEditorPCH.h>

namespace rczEngine
{
	EditorCore*& EditorCore::_Instance()
	{
		static EditorCore* instance = nullptr;
		return instance;
	}

	void EditorCore::Start()
	{
		(_Instance()) = new EditorCore;
	}

	EditorCore* EditorCore::Pointer()
	{
		return _Instance();
	}

	void EditorCore::ShutDown()
	{
		delete _Instance();
	}


	void EditorCore::InitEditor()
	{
		OSLayer::Start();
		OSLayer::Pointer()->InitOSLayer(GetModuleHandle(NULL));

		Logger::Start();

		Profiler::Start();

		Gfx::GfxCore::Start();
		Gfx::GfxCore::Pointer()->InitAndStart(true);

		Input::Start();
		Input::Pointer()->InitInput();
	
		//ComputeAPI::Start();
		//ComputeAPI::Pointer()->InitComputeAPI(Gfx::GfxCore::Pointer());

		//SoundAPI* SndAPI = new SoundAPI;
		//SndAPI->InitSound();

		ResVault::Start();
		ResVault::Pointer()->InitResourceManager();

		EventManager::Start();
		EventManager::Pointer()->InitEventManager();

		ImGUIEditor::Start();
		ImGUIEditor::Pointer()->InitEditor();

		GraphicDebugger::Start();
		GraphicDebugger::Pointer()->InitGDebugger();

		CameraManager::Start();
		CameraManager::Pointer()->Init(Gfx::GfxCore::Pointer());

		LightManager::Start();
		LightManager::Pointer()->InitLightManager();

		ActorComponentFactory::Start();

		SceneManager::Start();
		SceneManager::Pointer()->CreateDefaultScene("Default");
		m_scnManager = SceneManager::Pointer();

		RacrozRenderer::Start();
		RacrozRenderer::Pointer()->InitRenderer();
		m_renderer = RacrozRenderer::Pointer();

		Time.StartTimer();

		m_gfx = Gfx::GfxCore::Pointer();
	}

	void EditorCore::RunEditor()
	{
		bool editor = true;
		while (editor)
		{
			m_gfx->SetDefaultRenderTarget();

			m_gfx->ClearDefaultRenderTargetView(0.1f, 0.1f, 0.1f, 1.0f);

			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Carbon Engine");

			ImGui::Text("Projects");
			ImGui::Separator();
			ImGui::Text("Unit Testing");
			
			if (ImGui::Button("3D Math Unit Test"))
			{
				ImGui::End();
				ImGui::EndFrame();
				ImGui::Render();
				ImGUIEditor::Pointer()->PreRender(ImGui::GetDrawData());

				Math3DUnitTest();

				return;
			}

			ImGui::End();

			ImGui::EndFrame();

			ImGui::Render();
			ImGUIEditor::Pointer()->PreRender(ImGui::GetDrawData());

			m_gfx->Present();

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

		//while (Quit)
		//{
		//	//Quit = Input::GetKeyDown(KEY_ESCAPE);
		//	float deltaTime = (float)Time.GetFrameTime();
		//	
		//	Input::Pointer()->UpdateInput();
		//	m_scnManager->GetActiveScene()->Update(deltaTime);
		//
		//	m_renderer->Render(m_scnManager->GetActiveScene().get(), ImGUIEditor::Pointer());
		//
		//	// Bucle principal de mensajes:
		//	if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		//	{
		//		TranslateMessage(&msg);
		//		DispatchMessage(&msg);
		//		//if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
		//		//{
		//		//
		//		//}
		//	}
		//}
	}

	void EditorCore::InitSceneGrid()
	{
		auto gDebugger = GraphicDebugger::Pointer();

		Vector<Vector3> points;

		for (int i = 0; i < 100; ++i)
		{
			const auto pos = 1.0f * (i - 50);
			points.push_back(Vector3(pos, 0.0f, -50.0f));
			points.push_back(Vector3(pos, 0.0f, 50.0f));
		}

		for (int i = 0; i < 100; ++i)
		{
			const auto pos = 1.0f * (i - 50);
			points.push_back(Vector3(-50.0f, 0.0f, pos));
			points.push_back(Vector3(50.0f, 0.0f, pos));
		}

		Vector<uint32> indices;

		for (int i = 0; i < 400; i += 2)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
		}

		gDebugger->AddLineListIndex("Grid", points, indices, Color(0.4f, 0.4f, 0.4f), -1.0f);
	};

	void EditorCore::RenderGUI()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		for (auto& window : m_EditorWindows)
		{
			if (window.second->IsOpen())
				window.second->RenderWindow();
		}

		ImGui::EndFrame();
		ImGui::Render();
		ImGUIEditor::Pointer()->PreRender(ImGui::GetDrawData());
	}

	void EditorCore::UpdateWindows(float deltaTime)
	{
		for (auto& window : m_EditorWindows)
		{
			if (!window.second->IsOpen())
			{
				m_EditorWindows.erase(window.first);
				return;
			}
		}
	}

	void EditorCore::RenderEditor()
	{
		m_gfx->SetDefaultRenderTarget();
		m_gfx->ClearDefaultRenderTargetView(0.1f, 0.1f, 0.1f, 1.0f);
		m_gfx->ClearDepthTargetView();

		auto scene = SceneManager::Pointer()->GetActiveScene();

		m_renderer->Render("Scene", scene, nullptr);

		RenderGUI();

		m_gfx->Present();
	}

	void EditorCore::UpdateEditor()
	{
		float deltaTime = (float)Time.GetFrameTime();

		UpdateWindows(deltaTime);

		Input::Pointer()->UpdateInput();

		SceneManager::Pointer()->GetActiveScene()->Update(deltaTime);

		//auto cubepos = Vector3(2, 2, 0);
		//auto cubeSize = Vector3(0.5f, 0.5f, 0.5f);
		//auto cube = gDebugger->AddCube("Cube", cubepos, cubeSize, Color(1, 0, 0));
		//Quaternion q = Quaternion::FromAxisAngle(Vector3(0, 5, 5).GetNormalized(), 10);
		//q = Quaternion::FromAxisAngle(Vector3(0, 10, 10).GetNormalized(), 180 * deltaTime);
		//cubepos = q.RotateVector(cubepos, 1);
		//cube.lock()->SetCube(cubepos, Vector3(0.5f, 0.5f, 0.5f));

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

	void EditorCore::Math3DUnitTest()
	{
		InitSceneGrid();

		m_renderer->CreatePipeline("Scene", eRenderingPipelines::Deferred);

		auto window = new PipelineWindow();

		window->InitWindow("Scene");
		m_EditorWindows["Scene"] = window;
		window->SetRenderPipeline(m_renderer->GetPipeline("Scene").lock());


		auto sceneWindow = new ScenesWindow();
		sceneWindow->InitWindow("Scenes");
		m_EditorWindows["Scenes"] = sceneWindow;

		auto sceneGameObj = new GameObjectWindow();
		sceneGameObj->InitWindow("GameObject");
		m_EditorWindows["GameObject"] = sceneGameObj;

		auto sceneConsole = new ConsoleWindow();
		sceneConsole->InitWindow("Console");
		m_EditorWindows["Console"] = sceneConsole;

		auto sceneResourceList = new ResourceListWindow();
		sceneResourceList->InitWindow("Resource List");
		m_EditorWindows["Resource List"] = sceneResourceList;

		auto VXGI = new VXGIWindow();
		VXGI->InitWindow("VXGI");
		m_EditorWindows["VXGI"] = VXGI;

		auto gDebugger = GraphicDebugger::Pointer();
		
		bool editor = true;
		while (editor)
		{
			UpdateEditor();

			RenderEditor();
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

		ImGUIEditor::ShutDown();

		GraphicDebugger::ShutDown();

		Profiler::ShutDown();
		Logger::ShutDown();

		Gfx::GfxCore::ShutDown();
	}

	void EditorCore::CreateActiveResourceWindow(WeakPtr<Resource> resource)
	{
		if (resource.expired())
		{
			Logger::Pointer()->Log("Tried to create resource window with invalid ptr.", eLogMsgType::Warning);
			return;
		}

		auto window = new ResourceWindow();
		const auto windowName = "Resource " + resource.lock()->GetName();
		window->InitWindow(windowName.c_str(), 200, 400);
		window->SetResource(resource);
		m_EditorWindows[windowName] = window;
	}
}