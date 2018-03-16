#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT GUIEditor;

	namespace Core
	{
		class RZ_UTILITY_EXPORT App
		{
		public:
			virtual void StartApp() = 0;
			virtual void UpdateApp() = 0;
			virtual void CloseApp() = 0;

		private:

		};
	}
}