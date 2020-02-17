#pragma once

namespace rczEngine
{
#ifdef RZ_PLATFORM == RZ_PLATFORM_WINDOWS
	typedef HWND WindowHandle;
	typedef HINSTANCE InstanceHandle;
#endif
};