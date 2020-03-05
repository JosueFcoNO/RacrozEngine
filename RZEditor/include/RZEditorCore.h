#pragma once

namespace rczEngine
{
	enum class eEditorStates
	{
		Main,
		CreateProject,
		Project
	};

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
		void StartUIRender();
		void EndUIRender();
		void NewProjectWindow();
		void StartProject(const String & name);
		void CreateProject(const String & name, const StringW & path, RendererSettings renderer);
		void MainWindow();
		void RunEditor();
		void InitSceneGrid();
		void RenderGUI();
		void UpdateWindows(float deltaTime);
		void RenderEditor();
		void UpdateEditor();
		void Math3DUnitTest();
		void DestroyEditor();

		void CreateActiveResourceWindow(WeakPtr<Resource> resource);

		void SetActiveGameObject(WeakGameObjPtr activeGameObject) {
			m_ActiveGameObject = activeGameObject;
		};

		WeakGameObjPtr GetActiveGameObject() {
			return m_ActiveGameObject;
		};

		EditorSettings m_EditorSettings;

	private:
		bool Quit = false;
		Timer Time;

		Gfx::GfxCore* m_gfx;
		RacrozRenderer* m_renderer;
		SceneManager* m_scnManager;

		Map<String, BaseWindow*> m_EditorWindows;

		WeakGameObjPtr m_ActiveGameObject;
	
		eEditorStates m_State = eEditorStates::Main;

		Map<String, Project> m_Projects;

#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
		HACCEL hAccelTable;
		MSG msg;
#endif
	};

}