#pragma once
#include "CbnUtilitiesPCH.h"

namespace rczEngine
{
	OSLayer*& OSLayer::_Instance()
	{
		static OSLayer* instance = nullptr;
		return instance;
	}

	void OSLayer::Start()
	{
		(_Instance()) = new OSLayer;
	}

	OSLayer* OSLayer::Pointer()
	{
		return _Instance();
	}

	void OSLayer::ShutDown()
	{
		delete _Instance();
	}

	void OSLayer::InitOSLayer(InstanceHandle handle)
	{
		m_Instance = handle;

		const HWND hDesktop = GetDesktopWindow();
		RECT desktop;
		GetWindowRect(hDesktop, &desktop);

		auto window = CreateCbnWindow("Carbon Engine", 0, 0, desktop.right, desktop.bottom, Window_NormalWindow);

		auto rect = window.GetRect();

		ShowWindow(window.GetHandle(), SW_MAXIMIZE);

		rect = window.GetRect();

		UpdateWindow(window.GetHandle());
	}

	Window& OSLayer::CreateCbnWindow(const String& windowID, int x, int y, int width, int height, long windowType)
	{
		auto handle = CreateWindowA(m_WindowClass.c_str(), windowID.c_str(), windowType, x, y, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,  m_Instance, nullptr);
	
		m_WindowMap.insert(Pair<String, Window>(windowID, Window()));
		m_WindowMap[windowID].InitWindow(handle, std::move(windowID));

		return m_WindowMap[windowID];
	}

	InstanceHandle OSLayer::GetInstance()
	{
		return m_Instance;
	}

	WindowHandle OSLayer::GetWindowHandle(const String && windowID)
	{
		return m_WindowMap.at(windowID).GetHandle();
	}

};