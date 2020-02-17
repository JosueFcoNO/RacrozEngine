#pragma once

namespace rczEngine
{
	///The platform-indepent window class, holds the handle to the window, its name and its rect.
	class RZ_EXP Window
	{
	public:
		///Initialize the window.
		void InitWindow(WindowHandle wHandle, const String&& windowName);
		
		FORCEINLINE String GetName() const { return m_WindowName; };
		FORCEINLINE WindowHandle GetHandle() const { return m_WindowHandle; };
		FORCEINLINE RECT GetRect() { GetClientRect(m_WindowHandle, &m_WindowRect); return m_WindowRect; };

	private:
		RECT m_WindowRect;
		WindowHandle m_WindowHandle;
		String m_WindowName;
	};
}