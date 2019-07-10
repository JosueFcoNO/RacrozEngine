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

	HWND hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE, CW_USEDEFAULT, 0, 2560, 1440, nullptr, nullptr, hInstance, nullptr);

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
