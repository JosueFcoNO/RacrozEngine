// RZEngine-UnitTest.cpp: define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include "RZVulkanAPI-UnitTest.h"

#include <RZEnginePCH.h>
#include <RZRendererPCH.h>

#define MAX_LOADSTRING 100

// Variables globales:
HWND hWnd;
HINSTANCE hInst;                                // Instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal

using namespace rczEngine;

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


GraphicDebugger g_Debugger;
EventManager* EventMng = NULL;

struct UserConfig
{
	float Roughness;
	float Metallic;
	float padding;
	float padding1;

	Vector4 UserAlbedo;

	float OverrideAlbedo;
	float OverrideNormal;
	float OverrideMetallic;
	float OverrideRoughness;

	Vector3 UserSpecular;
	float OverrideSpecular;

	float LightIntensity;
	float AmbientLightIntensity;
	float ReflectionIntensity;
	float padding2;
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Inicializar cadenas globales
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_RZENGINEUNITTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Realizar la inicialización de la aplicación:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	RECT rc;
	GetClientRect(hWnd, &rc);

	//////////////////////////////////////////////////
	//////////////GfxCore Init////////////////////
	//////////////////////////////////////////////////


	GfxCore* API = new GfxCore();
	API->CreateDevice(hWnd, rc.right, rc.bottom, 1, 60, 1, 0, true);
	API->CreateDeviceContext();
	API->CreateSwapChain(hWnd);
	API->GetBackBufferInterface();
	API->SetRenderTargetViewAndDepthStencil();
	API->SetViewPort();

	ComputeAPI CAPI;
	//CAPI.InitComputeAPI(API);

	SoundAPI::StartUp();
	SoundAPI* SoundAPI = SoundAPI::InstancePtr();
	SoundAPI->InitSound();

	ResVault::StartUp();
	ResVault* RES = ResVault::InstancePtr();
	RES->graphAPI = API;
	RES->InitResourceManager();

	EventManager::StartUp();
	EventMng = EventManager::InstancePtr();
	EventMng->hinstance = hInst;
	EventMng->hwnd = hWnd;
	EventMng->InitEventManager();

	Scene Escena;
	Escena.Init(API, RES);

	Editor Edit;
	Edit.StartEditor(API, RES, EventMng, &Escena);

	//////////////////////////////////////////////////
	//////////////Renderer////////////////////////////
	//////////////////////////////////////////////////

	RacrozRenderer Racroz;
	Racroz.Init(API, RES);

	//////////////////////////////////////////////////
	//////////////Graphic Debugger////////////////////
	//////////////////////////////////////////////////

	g_Debugger.Init(RES);
	//for (float i = -49; i < 51; i += 1)
	//{
	//	g_Debugger.AddStaticDebuggerLine(Vector3(0, -50, 0), Vector3(0, 50, 0), .3f, .3f, .3f);
	//	g_Debugger.AddStaticDebuggerLine(Vector3(i, 0, 50), Vector3(i, 0, -50), .3f, .3f, .3f);
	//	g_Debugger.AddStaticDebuggerLine(Vector3(-50, 0, i), Vector3(50, 0, i), .3f, .3f, .3f);
	//}
	//
	//g_Debugger.AddStaticDebuggerLine(Vector3(0, 0, 50), Vector3(0, 0, -50), 1, 0, 0);
	//
	//g_Debugger.AddStaticDebuggerPoint(0, 0, 0, 0.1f, 1, 1, 1);

	//////////////////////////////////////////////////
	//////////////Objects/////////////////////////////
	//////////////////////////////////////////////////

	auto Mat = RES->GetResource<Material>(RES->GetResource<Model>(RES->m_ModelSphere).lock()->m_VectorMaterial[0]);

	Mat.lock()->m_TextureEm = RES->m_WhiteTex;

	ResourceHandle HighPolyCube = RES->LoadResource("Models/HighPolyCube.fbx", "HighCube");

	//ResourceHandle Revolver = RES->LoadResource("Models/Revolver/Revolver.dae", "Rev");
	//auto mat = RES->GetResource<Model>(Revolver).lock()->m_VectorMaterial[0];
	//auto Materiale = RES->GetResource<Material>(mat);
	//Materiale.lock()->SetTexture(eTextureType::BaseColor, RES->LoadResource("Models/Revolver/a.tga", "albedo"));
	//Materiale.lock()->SetTexture(eTextureType::AmbientOcclusion, RES->LoadResource("Models/Revolver/ao.tga", "ao"));
	//Materiale.lock()->SetTexture(eTextureType::Normals, RES->LoadResource("Models/Revolver/n.tga", "normal"));
	//Materiale.lock()->SetTexture(eTextureType::Emissive, RES->LoadResource("Models/Revolver/e.tga", "em"));
	//Materiale.lock()->SetTexture(eTextureType::Metallic, RES->LoadResource("Models/Revolver/m.tga", "met"));
	//Materiale.lock()->SetTexture(eTextureType::Roughness, RES->LoadResource("Models/Revolver/r.tga", "r"));

	//////////////////////////////////////////////////
	//////////////Scene///////////////////////////////
	//////////////////////////////////////////////////

	LightManager g_LightManager;
	g_LightManager.InitLightManager(API);

	Escena.m_light = &g_LightManager;

	float aspectRatio = float(API->GetWidth()) / float(API->GetHeight());

	StrGameObjectPtr ActorCamera = Escena.CreateActor("Camera", NULL, Vector3(0, 2, -250.0f));
	Camera* cam = &ActorCamera->AddComponent<CameraCmp>()->m_Camera;
	cam->Init(Vector3(0, 0, 300), Vector3(0, 0, 0), 0.1f, 1000.0f, aspectRatio + 0.1f);
	ActorCamera->AddComponent<SpaceTravel>()->InitSpaceTravel(cam, RES, EventMng);

	StrGameObjectPtr aPlanet = Escena.CreateActor("Planet", NULL, Vector3(0, -20, 0), Vector3(0, 0, 0), Vector3(100, 100, 100));
	SpaceManager* space = &aPlanet->AddComponent<SpaceComponent>()->m_Space;
	space->InitSpaceManager(ActorCamera->GetComponent<SpaceTravel>(CMP_SPACE_TRAVEL).get(), RES, API, &CAPI, &Escena);

	Racroz.m_Space = space;

	Edit.m_GameObjectProps->InitPlanet(space);

	space->CreatePlanet(61239, 0, 0, 0);

	StrGameObjectPtr aLight2 = Escena.CreateActor("Bang2", NULL, Vector3(15, 10, 10), Vector3(0, 0, 0), Vector3(1, 1, 1));
	aLight2->AddComponent<ModelRenderer>()->m_Model = RES->m_ModelCube;
	aLight2->AddComponent<LightComponent>()->InitLightInManager(&g_LightManager, API, eLIGHT_TYPE::LIGHT_DIR, Vector3(15, 10, 10), Vector3(-1, -1, 0), Vector4(1, 1, 1, 1.0f));

	//////////////////////////////////////////////////
	//////////////CAMARA MANAGER//////////////////////
	//////////////////////////////////////////////////

	CameraManager cManager;
	cManager.Init(API);
	cManager.AddCamera(ActorCamera->GetComponent<CameraCmp>(CMP_CAMERA));
	cManager.SetActiveCamera("Camera", API);

	//////////////////////////////////////////////////
	//////////////DISNEY BRDF/////////////////////////
	//////////////////////////////////////////////////

	UserConfig config;
	config.AmbientLightIntensity = 1.0f;
	config.LightIntensity = 1.0f;
	config.ReflectionIntensity = 1.0f;

	config.Metallic = 0.5f;
	config.OverrideAlbedo = 0.0f;
	config.OverrideMetallic = 0.0f;
	config.OverrideNormal = 0.0f;
	config.OverrideRoughness = 0.0f;
	config.OverrideSpecular = 0.0f;
	config.Roughness = 0.5f;
	config.UserAlbedo.Set(0.0f, 0.0f, 0.0f, 0.0f);
	config.UserSpecular.Set(0.0f, 0.0f, 0.0f);

	ConstantBuffer UserDisney;
	UserDisney.CreateConstantBuffer(sizeof(UserConfig), USAGE_DEFAULT, API);
	UserDisney.UpdateConstantBuffer(&config, API);
	UserDisney.SetBufferInPS(6, API);

	//////////////////////////////////////////////////
	//////////////CUBEMAPS////////////////////////////
	//////////////////////////////////////////////////

	CubeMap* CubeMaps[10];

	for (int i = 0; i < 10; ++i)
	{
		CubeMaps[i] = new CubeMap;
	}

	CubeMaps[0]->LoadCubeMapFromDDS("CubeMaps/Royal.dds", "Env", false, RES);

	CubeMaps[1]->LoadCubeMapFromDDS("CubeMaps/Yokohama.dds", "Env", false, RES);

	CubeMaps[2]->LoadCubeMapFromDDS("CubeMaps/Yokohama2.dds", "Env", false, RES);

	CubeMaps[3]->LoadCubeMapFromDDS("CubeMaps/Space.dds", "Env", false, RES);

	CubeMaps[4]->LoadCubeMapFromDDS("CubeMaps/DarkClouds.dds", "Env", false, RES);

	CubeMaps[5]->LoadCubeMapFromDDS("CubeMaps/RedClouds.dds", "Env", false, RES);

	CubeMaps[6]->LoadCubeMapFromDDS("CubeMaps/SpaceClouds.dds", "Env", false, RES);

	CubeMaps[7]->LoadCubeMapFromDDS("CubeMaps/BobEsponja.dds", "Env", false, RES);

	CubeMaps[8]->LoadCubeMapFromDDS("CubeMaps/TantoLundenSea.dds", "Env", false, RES);

	CubeMaps[9]->LoadCubeMapFromDDS("CubeMaps/CubePark.dds", "Env", false, RES);


	//////////////////////////////////////////////////
	//////////////DRAW////////////////////////////////
	//////////////////////////////////////////////////

	Timer Time;
	Time.StartTimer();

	StrPtr<CameraCmp> Temp = ActorCamera->GetComponent<CameraCmp>(CMP_CAMERA);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RZENGINEUNITTEST));

	MSG msg;

	ConstantBuffer SkyBoxBuffer;
	SkyBoxBuffer.CreateConstantBuffer(sizeof(Matrix4), USAGE_DEFAULT, API);

	int CurrentCubeMap = 0;
	Racroz.ChangeCubeMap(CubeMaps[3]);
	Racroz.FinalTexture();

	float DeltaButton = 0.25f;

	Matrix4 Sky(Unit);

	while (!EventMng->m_input.CheckKeyboardState(DIK_ESCAPE))
	{
		float m = (float)Time.GetFrameTime();

		bool Next = EventMng->m_input.CheckKeyboardState(DIK_RIGHT);
		bool Prev = EventMng->m_input.CheckKeyboardState(DIK_LEFT);
		bool Final = EventMng->m_input.CheckKeyboardState(DIK_F);

		bool NextCube = EventMng->m_input.CheckKeyboardState(DIK_O);
		bool PrevCube = EventMng->m_input.CheckKeyboardState(DIK_I);

		bool MipDown = EventMng->m_input.CheckKeyboardState(DIK_L);
		bool MipUp = EventMng->m_input.CheckKeyboardState(DIK_K);
		DeltaButton -= m;

		if (DeltaButton < 0.0f)
		{
			DeltaButton = 0.25f;

			if (Next)
				Racroz.NextTexture();
			else if (Prev)
				Racroz.PrevTexture();

			if (Final)
				Racroz.FinalTexture();

			if (NextCube)
			{
				++CurrentCubeMap;
				if (CurrentCubeMap == 10) CurrentCubeMap = 0;
				Racroz.ChangeCubeMap(CubeMaps[CurrentCubeMap]);
			}
			else if (PrevCube)
			{
				--CurrentCubeMap;
				if (CurrentCubeMap == -1) CurrentCubeMap = 9;
				Racroz.ChangeCubeMap(CubeMaps[CurrentCubeMap]);
			}
		}

		auto pln2 = aPlanet->GetComponent<SpaceComponent>(CMP_SPACE_MANAGER);


		//Edit.m_GameObjectProps->UpdatePoint(V);

		Vector3 pos = pln2->m_Space.m_Traveller->m_Camera->GetPosition().GetNormalized();
		pos = Math::SphereToCubePoint(pos) * 100;
		pos.mx = floorf(pos.mx) / 100.0f;
		pos.my = floorf(pos.my) / 100.0f;
		pos.mz = floorf(pos.mz) / 100.0f;

		Edit.planetScale = (pos.mx + 1.0f) * 25.0f;
		Edit.planetDist = (pos.my + 1.0f) * 25.0f;
		Edit.realDist = (pos.mz + 1.0f) * 25.0f;

		cam->m_Target = Vector3(0, 0, 0);

		Edit.UpdateEditor(m);
		EventMng->CheckEvents();
		Escena.Update(m);

		cManager.UpdateAndSetCameraBuffer(API, 5, 5);
		g_LightManager.SetLightsBuffers(API);


		Sky = Temp->GetViewMatrix();
		Sky.m_rows[3].Set(0, 0, 0, 1);
		Sky.Transpose();

		SkyBoxBuffer.UpdateConstantBuffer(&Sky, API);
		SkyBoxBuffer.SetBufferInPS(0, API);
		SkyBoxBuffer.SetBufferInVS(0, API);

		Racroz.Render(&Escena, Edit.GetCanvas());

		// Bucle principal de mensajes:
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

	}

	PostQuitMessage(0);
	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RZENGINEUNITTEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RZENGINEUNITTEST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Almacenar identificador de instancia en una variable global

	HWND hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
		CW_USEDEFAULT, 0, 1920, 1080, nullptr, nullptr, hInstance, nullptr);

	RECT rc;
	GetWindowRect(hwnd, &rc);

	if (!hwnd)
	{
		return FALSE;
	}
	hWnd = hwnd;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_SETFOCUS:
		if (EventMng)
			EventMng->m_input.RegainDevices();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		//DESTROY gAPI
		ResVault::Instance().Destroy();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
