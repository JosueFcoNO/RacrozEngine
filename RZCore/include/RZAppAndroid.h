#pragma once

namespace rczEngine
{
	namespace Core
	{
		class RZ_EXP AppAndroid : App
		{
		public:
			void StartApp() noexcept final {};
			void UpdateApp() noexcept final {};
			void CloseApp() noexcept final {};

		private:

		};
	}
}