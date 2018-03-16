#include <RZEnginePCH.h>

namespace rczEngine
{
	void Editor::StartEditor(Gfx::GfxCore * gfx, ResourceManager * res, EventManager* evnt, Scene* scene)
	{
		m_gfx = gfx;
		m_res = res;
		m_evnt = evnt;
		m_CurrentScene = scene;

		m_CanvasPantalla.Init(gfx->GetWidth(), gfx->GetHeight(), gfx, res, evnt);

		LoadResouces(res);

		m_GameObjectProps = new Editor_GameObjectProps;
		m_GameObjectProps->InitGameObjectProps(m_CurrentScene, this);

		//WeakPtr<UI_Element> ResourceBox = m_CanvasPantalla.CreateUI_Element("CuadroRes", m_Resources[ERES_RESOURCES], 420+810, gfx->GetHeight() - 340, 5, 394, 340);
		//m_CanvasPantalla.m_Root.AddChildren(ResourceBox);
		
		//WeakPtr<UI_Element> MaterialBox = m_CanvasPantalla.CreateUI_Element("MaterialBox", m_Resources[ERES_MATERIAL], 420, gfx->GetHeight() - 340, 3, 813, 340);
		//m_CanvasPantalla.m_Root.AddChildren(MaterialBox);
		//
		//WeakPtr<UI_Element> TextResources = m_CanvasPantalla.CreateUI_Text(L"Resources", "TextResource", 20, 5, 24, 24);
		//ResourceBox.lock()->AddChildren(TextResources);

		FPSText = m_CanvasPantalla.CreateUI_Text(TextTool::WTextFloatTo(L"FPS: ", m_fps), "FPS", 5, 10, 30, 30);
		m_CanvasPantalla.m_Root.AddChildren(FPSText);

		PlanetScale = m_CanvasPantalla.CreateUI_Text(TextTool::WTextFloatTo(L"Planet Size: ", m_fps), "MX", 5, 50, 30, 30);
		m_CanvasPantalla.m_Root.AddChildren(PlanetScale);

		PlanetDist = m_CanvasPantalla.CreateUI_Text(TextTool::WTextFloatTo(L"Space Distance: ", m_fps), "MY", 5, 85, 30, 30);
		m_CanvasPantalla.m_Root.AddChildren(PlanetDist);

		RealDist = m_CanvasPantalla.CreateUI_Text(TextTool::WTextFloatTo(L"Planet Scale: ", m_fps), "MY", 5, 115, 30, 30);
		m_CanvasPantalla.m_Root.AddChildren(RealDist);
	}

	void Editor::UpdateEditor(float deltaTime)
	{
		m_second += deltaTime;
		++m_fps;

		if (m_second >= 1.0f)
		{
			UI_Text* ptr = (UI_Text*)(FPSText.lock().get());
			ptr->SetText(TextTool::WTextFloatTo(L"FPS: ", m_fps));
			m_fps = 0;
			m_second = 0.0f;
		}

		auto 
		DebugText = (UI_Text*)(PlanetScale.lock().get());
		DebugText->SetText(TextTool::WTextFloatTo(L"Planet Size: : ", planetScale));

		DebugText = (UI_Text*)(PlanetDist.lock().get());
		DebugText->SetText(TextTool::WTextFloatTo(L"Space Distance: ", planetDist));

		DebugText = (UI_Text*)(RealDist.lock().get());
		DebugText->SetText(TextTool::WTextFloatTo(L"Terrain Scale: ", realDist));

		m_CanvasPantalla.UpdateCanvas(deltaTime);
	}

	void Editor::RenderEditor()
	{

	}

	ResourceHandle Editor::GetUIResource(EDITOR_RESOURCE edtResource)
	{
		return m_Resources[edtResource];
	}

	ResourceHandle Editor::LoadFile(char* instruction, char* name, ResourceManager* res)
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
		ofn.lpstrTitle = instruction;
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn))
		{
			return res->LoadResource(filename, name);
		}
		else
		{
			switch (CommDlgExtendedError())
			{
			default: std::cout << "You cancelled.\n";
			}
		}

		return INVALID_RESOURCE;
	}

	void Editor::ChangeActiveGameObject(WeakPtr<GameObject> gameObj, UI_Button* uiButton)
	{
		if (m_ActiveGameObjectUI)
		{
			m_ActiveGameObjectUI->m_TextureIdle = m_Resources[ERES_GAMEOBJECT_IDLE];
			m_ActiveGameObjectUI->m_Texture = m_ActiveGameObjectUI->m_TextureIdle;
		}


		m_ActiveGameObject = gameObj;
		m_ActiveGameObjectUI = uiButton;

		m_GameObjectProps->ChangedObject();
	}

	void Editor::LoadResouces(ResourceManager * res)
	{
		for (int32 i = 0; i < ERES_TOTAL; ++i)
		m_Resources.push_back(0);

		m_Resources[ERES_BTN1_IDLE] = res->LoadResource("RacrozEngineAssets/btn1Idle.png");
		m_Resources[ERES_BTN1_HOVER] = res->LoadResource("RacrozEngineAssets/btn1Hover.png");
		m_Resources[ERES_BTN1_PRESSED] = res->LoadResource("RacrozEngineAssets/btn1Pressed.png");

		m_Resources[ERES_SCENEGRAPH] = res->LoadResource("RacrozEngineAssets/SceneGraph.png");
		m_Resources[ERES_RESOURCES] = res->LoadResource("RacrozEngineAssets/ResourcesProps.png");
		m_Resources[ERES_MATERIAL] = res->LoadResource("RacrozEngineAssets/Material.png");
		m_Resources[ERES_GAMEOBJECT_PROPS] = res->LoadResource("RacrozEngineAssets/ActorMng.png");

		m_Resources[ERES_GAMEOBJECT_IDLE] = res->LoadResource("RacrozEngineAssets/ActorIdle.png");
		m_Resources[ERES_GAMEOBJECT_HOVER] = res->LoadResource("RacrozEngineAssets/ActorHover.png");
		m_Resources[ERES_GAMEOBJECT_PRESSED] = res->LoadResource("RacrozEngineAssets/ActorPressed.png");
		m_Resources[ERES_GAMEOBJECT_SELECTED] = res->LoadResource("RacrozEngineAssets/ActorSelected.png");

		m_Resources[ERES_EXIT_IDLE] = res->LoadResource("RacrozEngineAssets/btnExit.png");
		m_Resources[ERES_EXIT_HOVER] = res->LoadResource("RacrozEngineAssets/btnExitHover.png");
		m_Resources[ERES_EXIT_PRESSED] = res->LoadResource("RacrozEngineAssets/btnExitPressed.png");

		m_Resources[ERES_MIN_IDLE] = res->LoadResource("RacrozEngineAssets/btnMin.png");
		m_Resources[ERES_MIN_HOVER] = res->LoadResource("RacrozEngineAssets/btnMinHover.png");
		m_Resources[ERES_MIN_PRESSED] = res->LoadResource("RacrozEngineAssets/btnMinPressed.png");

		m_Resources[ERES_MAX_IDLE] = res->LoadResource("RacrozEngineAssets/btnMax.png");
		m_Resources[ERES_MAX_HOVER] = res->LoadResource("RacrozEngineAssets/btnMaxHover.png");
		m_Resources[ERES_MAX_PRESSED] = res->LoadResource("RacrozEngineAssets/btnMaxPressed.png");

		m_Resources[ERES_UP_IDLE] = res->LoadResource("RacrozEngineAssets/btnUp.png");
		m_Resources[ERES_UP_HOVER] = res->LoadResource("RacrozEngineAssets/btnUpHover.png");
		m_Resources[ERES_UP_PRESSED] = res->LoadResource("RacrozEngineAssets/btnUpPressed.png");

		m_Resources[ERES_DOWN_IDLE] = res->LoadResource("RacrozEngineAssets/btnDown.png");
		m_Resources[ERES_DOWN_HOVER] = res->LoadResource("RacrozEngineAssets/btnDownHover.png");
		m_Resources[ERES_DOWN_PRESSED] = res->LoadResource("RacrozEngineAssets/btnDownPressed.png");

		m_Resources[ERES_NEW_IDLE] = res->LoadResource("RacrozEngineAssets/btnNew.png");
		m_Resources[ERES_NEW_HOVER] = res->LoadResource("RacrozEngineAssets/btnNewHover.png");
		m_Resources[ERES_NEW_PRESSED] = res->LoadResource("RacrozEngineAssets/btnNewPressed.png");

		m_Resources[ERES_SLIDER] = res->LoadResource("RacrozEngineAssets/Slide.png");

		m_Resources[ERES_SLIDER_POINT] = res->LoadResource("RacrozEngineAssets/SlidePoint.png");
		m_Resources[ERES_SLIDER_POINT_HOVER] = res->LoadResource("RacrozEngineAssets/SlidePointHover.png");
		m_Resources[ERES_SLIDER_POINT_PRESSED] = res->LoadResource("RacrozEngineAssets/SlidePointPressed.png");

		m_Resources[ERES_SLIDER_MAX] = res->LoadResource("RacrozEngineAssets/MaxSlide.png");
		m_Resources[ERES_SLIDER_MIN] = res->LoadResource("RacrozEngineAssets/MinSlide.png");

	}

	void Editor::UnloadResources(ResourceManager * res)
	{

	}

};