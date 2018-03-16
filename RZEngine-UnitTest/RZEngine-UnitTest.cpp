// RZEngine-UnitTest.cpp: define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include "RZEngine-UnitTest.h"

#include <RZEditorPCH.h>

#define MAX_LOADSTRING 100

HWND hWnd;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

using namespace rczEngine;
bool Quit = false;

WeakGameObjectPtr LoadMat(float x, float y, float z, Scene* scene, char* matName, char* format, ResVault* RES, bool UVfix)
{
	auto Return = (UVfix) ? RES->LoadResource("Models/Mat/matuv.dae", "mat")  : RES->LoadResource("Models/Mat/mat.dae", "mat");

	auto Modele = RES->GetResource<Model>(Return).lock();

	WeakGameObjectPtr matActor = scene->CreateActor(matName, NULL, Vector3(x, y, z), Vector3(0, 0, 0), (UVfix) ? Vector3(.01f, .01f, .01f) : Vector3(1, 1,1));
	auto ModelMaterial = matActor.lock()->AddComponent<ModelRenderer>();
	ModelMaterial.lock()->SetModel(Return, RES);

	String PathBase("Models/Mat/");
	PathBase += matName;
	PathBase += "/Base_";

	String PathBody("Models/Mat/");
	PathBody += matName;
	PathBody += "/Body_";

	String PathHead("Models/Mat/");
	PathHead += matName;
	PathHead += "/Head_";

	String alb("a");
	String nor("n");
	String met("m");
	String rof("r");
	String ao("ao");
	String em("em");

	alb += format;
	nor += format;
	met += format;
	rof += format;
	ao += format;
	em += format;

	/////////////////
	//BASE MATERIAL//
	/////////////////

	auto BaseMat = new Material();
	BaseMat->InitMaterial(MAT_PBR_MetRough, scene->m_gfx);
	BaseMat->m_Name = "Base";

	ModelMaterial.lock()->OverrideMaterial(RES->InsertResource(BaseMat), "Base");

	///ALBEDO
	if (RES->FileExists((char*)(PathBase + alb).c_str()))
		BaseMat->m_TextureAlbedo = RES->LoadResource((char*)(PathBase + alb).c_str(), "a");

	///NORMAL
	if (RES->FileExists((char*)(PathBase + nor).c_str()))
		BaseMat->m_TextureNormal = RES->LoadResource((char*)(PathBase + nor).c_str(), "n");

	///MET
	if (RES->FileExists((char*)(PathBase + met).c_str()))
		BaseMat->m_TextureMetSpec = RES->LoadResource((char*)(PathBase + met).c_str(), "m");

	///Rof
	if (RES->FileExists((char*)(PathBase + rof).c_str()))
		BaseMat->m_TextureRoughSmooth = RES->LoadResource((char*)(PathBase + rof).c_str(), "r");

	///ao
	if (RES->FileExists((char*)(PathBase + ao).c_str()))
		BaseMat->m_TextureAO = RES->LoadResource((char*)(PathBase + ao).c_str(), "ao");

	///em
	if (RES->FileExists((char*)(PathBase + em).c_str()))
		BaseMat->m_TextureEm = RES->LoadResource((char*)(PathBase + em).c_str(), "em");

	/////////////////
	//BODY MATERIAL//
	/////////////////

	auto BodyMat = new Material();
	BodyMat->InitMaterial(MAT_PBR_MetRough, scene->m_gfx);
	BodyMat->m_Name = "Body";

	ModelMaterial.lock()->OverrideMaterial(RES->InsertResource(BodyMat), "Body");

	PathBase = PathBody;

	///ALBEDO
	if (RES->FileExists((char*)(PathBase + alb).c_str()))
		BodyMat->m_TextureAlbedo = RES->LoadResource((char*)(PathBase + alb).c_str(), "a");

	///NORMAL
	if (RES->FileExists((char*)(PathBase + nor).c_str()))
		BodyMat->m_TextureNormal = RES->LoadResource((char*)(PathBase + nor).c_str(), "n");

	///MET
	if (RES->FileExists((char*)(PathBase + met).c_str()))
		BodyMat->m_TextureMetSpec = RES->LoadResource((char*)(PathBase + met).c_str(), "m");

	///Rof
	if (RES->FileExists((char*)(PathBase + rof).c_str()))
		BodyMat->m_TextureRoughSmooth = RES->LoadResource((char*)(PathBase + rof).c_str(), "r");

	///ao
	if (RES->FileExists((char*)(PathBase + ao).c_str()))
		BodyMat->m_TextureAO = RES->LoadResource((char*)(PathBase + ao).c_str(), "ao");

	///em
	if (RES->FileExists((char*)(PathBase + em).c_str()))
		BodyMat->m_TextureEm = RES->LoadResource((char*)(PathBase + em).c_str(), "em");

	/////////////////
	//HEAD MATERIAL//
	/////////////////

	auto HeadMat = new Material();
	HeadMat->InitMaterial(MAT_PBR_MetRough, scene->m_gfx);
	HeadMat->m_Name = "Head";

	ModelMaterial.lock()->OverrideMaterial(RES->InsertResource(HeadMat), "Head");

	PathBase = PathHead;

	///ALBEDO
	if (RES->FileExists((char*)(PathBase + alb).c_str()))
		HeadMat->m_TextureAlbedo = RES->LoadResource((char*)(PathBase + alb).c_str(), "a");

	///NORMAL
	if (RES->FileExists((char*)(PathBase + nor).c_str()))
		HeadMat->m_TextureNormal = RES->LoadResource((char*)(PathBase + nor).c_str(), "n");

	///MET
	if (RES->FileExists((char*)(PathBase + met).c_str()))
		HeadMat->m_TextureMetSpec = RES->LoadResource((char*)(PathBase + met).c_str(), "m");

	///Rof
	if (RES->FileExists((char*)(PathBase + rof).c_str()))
		HeadMat->m_TextureRoughSmooth = RES->LoadResource((char*)(PathBase + rof).c_str(), "r");

	///ao
	if (RES->FileExists((char*)(PathBase + ao).c_str()))
		HeadMat->m_TextureAO = RES->LoadResource((char*)(PathBase + ao).c_str(), "ao");

	///em
	if (RES->FileExists((char*)(PathBase + em).c_str()))
		HeadMat->m_TextureEm = RES->LoadResource((char*)(PathBase + em).c_str(), "em");

	return matActor;
}

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

	std::unique_ptr<PlatformData> PlatformSpecificData = std::make_unique<PlatformData>();
	PlatformSpecificData->WindowHandle = hWnd;
	PlatformSpecificData->HandleInstance = hInst;

	EditorCore editor;
	editor.InitEditor(PlatformSpecificData.get());

	editor.RunEditor();

	PostQuitMessage(0);
	return 0;
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

	HWND hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE, CW_USEDEFAULT, 0, 1920, 1080, nullptr, nullptr, hInstance, nullptr);

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

extern LRESULT rczEngine::ImguiWndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (rczEngine::ImguiWndProcHandler(hWnd, message, wParam, lParam))
		return true;

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
		if (Input::Pointer())
		Input::Pointer()->RegainDevices();
		break;
	case WM_DESTROY:
		Quit = true;
		ResVault::Pointer()->Destroy();
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
