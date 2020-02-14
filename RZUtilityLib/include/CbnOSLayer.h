#pragma once

namespace rczEngine
{
	///Manages all interfacing with the OS. Windows, window input, update and destroy.
	class RZ_EXP OSLayer
	{
	private:
		static OSLayer*& _Instance();

	public:
		static void Start();
		static OSLayer* Pointer();
		static void ShutDown();

	public:
		///Inits the OS Layer, just needs the handle.
		void InitOSLayer(InstanceHandle handle);

		///Create a window and get the window handle.
		Window & CreateCbnWindow(const String& windowID, int x, int y, int width, int height, long windowType);

		///Returns the handle.
		InstanceHandle GetInstance();

		Window& GetWindow(const String & windowID = "Carbon Engine") { return m_WindowMap[windowID]; };

		///Get the window handle by name.
		WindowHandle GetWindowHandle(const String&& windowID = "Carbon Engine");

	private:
		///The map containing all windows.
		Map<String, Window> m_WindowMap;

		///The instance handle of the application.
		InstanceHandle m_Instance;

		///The window class to create more windows.
		String m_WindowClass = "CarbonClass";

		static const long Window_NormalWindow = WS_OVERLAPPEDWINDOW;
		static const long Window_PopUpWindow = WS_POPUPWINDOW;
	};
};