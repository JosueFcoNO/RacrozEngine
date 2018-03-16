#pragma once

namespace rczEngine
{
	namespace Core
	{
		class RZ_UTILITY_EXPORT AppWindows : public App
		{
		public:
			void StartApp() {};
			void UpdateApp() {};
			void CloseApp() {};

			HWND m_WindowHandle;
		};
	}
}