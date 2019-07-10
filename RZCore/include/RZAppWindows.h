#pragma once

namespace rczEngine
{
	namespace Core
	{
		class RZ_EXP AppWindows : public App
		{
		public:
			void StartApp() noexcept final {};
			void UpdateApp() noexcept final {};
			void CloseApp() noexcept final {};

			HWND m_WindowHandle = nullptr;
		};
	}
}