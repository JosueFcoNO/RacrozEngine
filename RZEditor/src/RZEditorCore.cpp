#include <RZEditorPCH.h>

#pragma once
#include <ShObjIdl.h>
#include <ShObjIdl_core.h>

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

	String LoadFile(const String& instruction, const String& name)
	{
		ANSICHAR filename[MAX_PATH];

		OPENFILENAME ofn;
		ZeroMemory(filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
		ofn.lpstrFilter = "Any File\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = instruction.c_str();
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn))
		{
		}
		else
		{
		}

		return "";
	};

	BasicString<wchar_t> PickFolder()
	{
		BasicString<WCHAR> path;

		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

		LPWSTR* string = (LPWSTR*)malloc(sizeof(WCHAR) * 512);

		IFileDialog *pfd;
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
		{
			DWORD dwOptions;
			if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
			{
				pfd->SetOptions(dwOptions | FOS_FORCEFILESYSTEM  | FOS_PICKFOLDERS);
			}
			if (SUCCEEDED(pfd->Show(NULL)))
			{
				IShellItem *psi;
				if (SUCCEEDED(pfd->GetResult(&psi)))
				{
					if (!SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, string)))
					{
						MessageBox(NULL, "GetIDListName() failed", NULL, NULL);
					}
					psi->Release();
				}
			}
			pfd->Release();
		}


		path.append(*string);
		delete string;

		return path;
	}

	void EditorCore::StartUIRender()
	{
		m_gfx->SetDefaultRenderTarget();
		m_gfx->ClearDefaultRenderTargetView(0.1f, 0.1f, 0.1f, 1.0f);

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Carbon Engine");
	}

	void EditorCore::EndUIRender()
	{
		ImGui::End();
		ImGui::EndFrame();
		ImGui::Render();
		ImGUIEditor::Pointer()->PreRender(ImGui::GetDrawData());
	}

	void EditorCore::NewProjectWindow()
	{
		static BasicString<wchar_t> pathProject;
		static String nameProject("--------------------------------");

		static char PathStringBuffer[1024];

		ImGui::InputText("Project Name: ", &nameProject[0], 32);

		ImGui::InputText("Project Folder: ", PathStringBuffer, 1024);
		ImGui::SameLine();
		if (ImGui::Button("Browse Project Folder"))
		{
			pathProject = PickFolder();
			strcpy(PathStringBuffer, TextTool::UniToAnsi(pathProject.c_str()).c_str());
		}

		if (ImGui::Button("Cancel"))
		{
			m_State = eEditorStates::Main;
		}

		if (ImGui::Button("Create"))
		{
			CreateProject(nameProject, pathProject, RendererSettings());
			m_EditorSettings.m_ProjectFilesScores[nameProject]  = ProjectRecord(nameProject, pathProject);
			m_EditorSettings.SaveEditorSettings();

			m_State = eEditorStates::Project;
		}
	}

	void EditorCore::StartProject(const String& name)
	{
		
	}

	void EditorCore::CreateProject(const String& name, const StringW& path, RendererSettings renderer)
	{
		m_Projects[name];

		m_Projects[name].SetName(name);
		m_Projects[name].SetPath(path);

		m_Projects[name].SaveCbn();
	}

	void EditorCore::MainWindow()
	{
		static BasicString<wchar_t> str;
		static String strProject;

		if (ImGui::Button("New Project"))
		{
			m_State = eEditorStates::CreateProject;
			Input::Pointer()->ReleaseDevices();
		}

		if (ImGui::Button("Open Project"))
		{
			strProject = GetFilePath("Choose Project");
		}

		ImGui::Separator();
		ImGui::Text("Projects");
		ImGui::Separator();

		for (auto projectRecord : m_EditorSettings.m_ProjectFilesScores)
		{
			ImGui::Button(projectRecord.first.c_str());

			ImGui::SameLine();

			ImGui::Text(TextTool::UniToAnsi(projectRecord.second.Path.c_str()).c_str());
		}

		//if (ImGui::Button("3D Math Unit Test"))
		//{
		//	Math3DUnitTest();
		//
		//	return;
		//}
	}

	void EditorCore::RunEditor()
	{
		m_EditorSettings.InitEditorSettings();

		bool editor = true;
		while (editor)
		{
			StartUIRender();

			switch (m_State)
			{
			case rczEngine::eEditorStates::Main:
				MainWindow();
				break;
			case rczEngine::eEditorStates::CreateProject:
				NewProjectWindow();
				break;
			case rczEngine::eEditorStates::Project:
				break;
			default:
				break;
			}

			EndUIRender();

			m_gfx->Present();

			// Bucle principal de mensajes:
			if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
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
		//InitSceneGrid();

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