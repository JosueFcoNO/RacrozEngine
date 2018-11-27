#pragma once

namespace rczEngine
{
	class RZ_EXP EditorCore
	{
	public:
		~EditorCore() { DestroyEditor(); };

		void InitEditor(PlatformData* platformData);
		void RunEditor();
		void DestroyEditor();

	private:
		bool Quit = false;
		Timer Time;

		Gfx::GfxCore* m_gfx;
		RacrozRenderer* m_renderer;
		SceneManager* m_scnManager;
		
#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
		HACCEL hAccelTable;
		MSG msg;
#endif
	};

}