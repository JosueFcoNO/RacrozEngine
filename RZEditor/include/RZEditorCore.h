#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT EditorCore
	{
	public:
		~EditorCore() { DestroyEditor(); };

		void InitEditor(PlatformData* platformData);
		void RunEditor();
		void DestroyEditor();

	private:
		bool Quit = false;
		Timer Time;
		
#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
		HACCEL hAccelTable;
		MSG msg;
#endif
	};

}