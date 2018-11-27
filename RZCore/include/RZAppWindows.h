#pragma once

namespace rczEngine
{
	namespace Core
	{
		class RZ_EXP AppWindows : public App
		{
		public:
			void StartApp() {};
			void UpdateApp() {};
			void CloseApp() {};

			HWND m_WindowHandle;
		};
	}
}