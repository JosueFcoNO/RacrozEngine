#pragma once

namespace rczEngine
{
	class RZ_EXP EditorCore
	{
	private:
		static EditorCore*& _Instance();

	public:
		static void Start();
		static EditorCore* Pointer();
		static void ShutDown();

	public:
		void InitEditor();
		void RunEditor();
		void InitSceneGrid();
		void RenderGUI();
		void RenderEditor();
		void UpdateEditor();
		void Math3DUnitTest();
		void DestroyEditor();

		void SetActiveGameObject(WeakGameObjPtr activeGameObject) {
			m_ActiveGameObject = activeGameObject;
		};

		WeakGameObjPtr GetActiveGameObject() {
			return m_ActiveGameObject;
		};

	private:
		bool Quit = false;
		Timer Time;

		Gfx::GfxCore* m_gfx;
		RacrozRenderer* m_renderer;
		SceneManager* m_scnManager;

		Map<String, BaseWindow*> m_EditorWindows;

		WeakGameObjPtr m_ActiveGameObject;
		
#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
		HACCEL hAccelTable;
		MSG msg;
#endif
	};

}