#pragma
#include "CbnUtilitiesPCH.h"

namespace rczEngine
{
	void Window::InitWindow(WindowHandle wHandle, const String && windowName)
	{
		m_WindowName = windowName;
		m_WindowHandle = wHandle;
	}
};