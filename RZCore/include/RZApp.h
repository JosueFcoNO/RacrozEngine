#pragma once

namespace rczEngine
{
	class RZ_EXP GUIEditor;

	namespace Core
	{
		class RZ_EXP App
		{
		public:
			virtual void StartApp() = 0;
			virtual void UpdateApp() = 0;
			virtual void CloseApp() = 0;

		private:

		};
	}
}