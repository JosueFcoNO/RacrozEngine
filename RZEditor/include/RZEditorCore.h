#pragma once

namespace rczEngine
{
	class RZ_EXP EditorCore
	{
	public:
		void InitEditor();
		void InitEngine();
		void RunEditor();
		void Math3DUnitTest();
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