#include <RZEnginePCH.h>

namespace rczEngine
{
#pragma region Static Functions

	ImGUIEditor*& ImGUIEditor::_Instance()
	{
		static ImGUIEditor* instance = nullptr;
		return instance;
	}

	void ImGUIEditor::Start()
	{
		(_Instance()) = new ImGUIEditor;
	}

	ImGUIEditor* ImGUIEditor::Pointer()
	{
		return _Instance();
	}

	void ImGUIEditor::ShutDown()
	{
		delete _Instance();
	}

	bool IsAnyMouseButtonDown()
	{
		ImGuiIO& io = ImGui::GetIO();
		for (int n = 0; n < ARRAYSIZE(io.MouseDown); n++)
			if (io.MouseDown[n])
				return true;
		return false;
	}
	
#pragma endregion

	void ImGUIEditor::InitEditor()
	{
		m_gfx = Gfx::GfxCore::Pointer();

		m_Hwnd = OSLayer::Pointer()->GetWindowHandle();

		InitImgui();

		return;
	}

	void ImGUIEditor::PreRender(ImDrawData * draw_data)
	{
		auto gfxCore = Gfx::GfxCore::Pointer();
		auto dContext = gfxCore->GetDeviceContext();
		gfxCore->SetDefaultRenderTarget();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGUIEditor::DestroyEditor()
	{
		m_Hwnd = (HWND)0;
	}

	void ImGUIEditor::RenderEditor(RendererConfig * user)
	{
		FPS = ImGui::GetIO().Framerate;

		NewFrame(m_gfx);
		{
			ImGui::Text("PBR Config");

			//ImGui::SliderFloat("Light", &user->LightIntensity, 0.0f, 1.0f);
			//ImGui::SliderFloat("Ambient", &user->AmbientLightIntensity, 0.0f, 1.0f);
			//ImGui::SliderFloat("Reflection", &user->ReflectionIntensity, 0.0f, 1.0f);
			//
			//ImGui::SliderInt("R", &user->R, 0, 1);
			//ImGui::SliderInt("G", &user->G, 0, 1);
			//ImGui::SliderInt("B", &user->B, 0, 1);

			ImGui::Checkbox("Wireframe", &Wireframe);

			ImGui::SliderInt("Skin Transform Mode:", &SkinMode, 0, 5);
			ImGui::Checkbox("Joint", &TransposeJoint);
			ImGui::Checkbox("ByOffset", &ByOffset);
			ImGui::Checkbox("UseParents", &UseParents);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / FPS, FPS);

			ImGui::SliderInt("Correction Mode:", &ColorMode, 0, 3);
			ImGui::SliderFloat("Exposure", &Exposure, 0.1f, 5.0f);
		}


		ImGui::Begin("Play Mode");
		{
			static bool PlayMode = false;

			if (ImGui::Checkbox("", &PlayMode))
			{
				if (PlayMode)
					Input::Pointer()->RegainDevices();
				else
					Input::Pointer()->ReleaseDevices();
			}
		}

		for (auto it = m_Windows.begin(); it != m_Windows.end(); ++it)
		{
			it->second->RenderWindow();
		}

	}

	void ImGUIEditor::InitImgui()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;
#if 0
		io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
		io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI
#endif

	// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(OSLayer::Pointer()->GetWindowHandle());
		ImGui_ImplDX11_Init(Gfx::GfxCore::Pointer()->GetDevice(), Gfx::GfxCore::Pointer()->GetDeviceContext());
	};

	void ImGUIEditor::NewFrame(Gfx::GfxCore * gfx)
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

};