#include <RZEditorPCH.h>

namespace rczEngine
{
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

		Time.StartTimer();

		m_gfx = Gfx::GfxCore::Pointer();

		//FbxLoader loader;
		//loader.LoadModel();
	}

	void EditorCore::InitEngine()
	{
		LightManager::Start();
		LightManager::Pointer()->InitLightManager();

		ActorComponentFactory::Start();

		SceneManager::Start();
		SceneManager::Pointer()->CreateDefaultScene("Default");
		m_scnManager = SceneManager::Pointer();

		RacrozRenderer::Start();
		RacrozRenderer::Pointer()->InitRenderer();
		m_renderer = RacrozRenderer::Pointer();

		StrPtr<SkyBox> Sky = std::make_shared<SkyBox>();
		Sky->InitSkyBox("CubeMaps/Restaurant.dds", "Env", Gfx::GfxCore::Pointer(), ResVault::Pointer());
		RacrozRenderer::Pointer()->ChangeSkyBox(Sky);

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

		while (Quit)
		{
			//Quit = Input::GetKeyDown(KEY_ESCAPE);
			float deltaTime = (float)Time.GetFrameTime();
			
			Input::Pointer()->UpdateInput();
			m_scnManager->GetActiveScene()->Update(deltaTime);

			m_renderer->Render(m_scnManager->GetActiveScene().get(), ImGUIEditor::Pointer());

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

	void EditorCore::Math3DUnitTest()
	{
		ActorComponentFactory::Start();

		SceneManager::Start();
		SceneManager::Pointer()->CreateDefaultScene("Default");
		m_scnManager = SceneManager::Pointer();

		RacrozRenderer::Start();
		RacrozRenderer::Pointer()->InitRenderer();
		m_renderer = RacrozRenderer::Pointer();

		m_renderer->SetRenderingMode(eRenderingPipelines::Debug);

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

		for (int i = 0; i < 400; i+=2)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
		}

		gDebugger->AddLineListIndex("Grid", points, indices, Color(0.4f, 0.4f, 0.4f), -1.0f);

		auto cube = gDebugger->AddCube("Cube", Vector3(10, 0, 0), Vector3(0.5f, 0.5f, 0.5f), Color(1, 0, 0));
		auto cubepos = Vector3(10, 0, 0);
		Quaternion q = Matrix3::Rotate3D(0, 5, 0).GetAsQuaternion();

		bool editor = true;
		while (editor)
		{
			float deltaTime = (float)Time.GetFrameTime();

			Input::Pointer()->UpdateInput();

			m_scnManager->GetActiveScene()->Update(deltaTime);
			
			cubepos = q.RotateVector(cubepos, 1);
			cube.lock()->SetCube(cubepos, Vector3(0.5f, 0.5f, 0.5f));

			m_gfx->SetDefaultRenderTarget();
			m_gfx->ClearDefaultRenderTargetView(0.1f, 0.1f, 0.1f, 1.0f);
			m_gfx->ClearDepthTargetView();
			m_renderer->Render(nullptr, nullptr);

			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::SetNextWindowSize(ImVec2(1920, 1080));
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::Begin("Editor",0, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse || ImGuiWindowFlags_::ImGuiWindowFlags_NoMove || ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
			ImGui::End();

			ImGui::SetNextWindowSize(ImVec2(960, 560));
			ImGui::Begin("Scene");
			ImGui::Image(m_renderer->GetCurrentPipeline().lock()->GetFinalRenderTarget().lock()->GetTextureCore()->m_ShaderResource, ImVec2(960, 540));
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
}