#pragma once

namespace rczEngine
{
	class RZ_EXP EditorCore
	{
	public:
		void InitEditor();
		void RunEditor();
		void InitSceneGrid();
		void RenderGUI();
		void RenderEditor();
		void UpdateEditor();
		void Math3DUnitTest();
		void DestroyEditor();

	private:
		bool Quit = false;
		Timer Time;

		Gfx::GfxCore* m_gfx;
		RacrozRenderer* m_renderer;
		SceneManager* m_scnManager;

		Map<String, BaseWindow*> m_EditorWindows;
		
#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
		HACCEL hAccelTable;
		MSG msg;
#endif
	};

}