#pragma once


namespace rczEngine
{
	class Editor_GameObject;
	class Editor_SceneGraph;
	class Editor_GameObjectProps;

	enum EDITOR_RESOURCE
	{
		ERES_SCENEGRAPH,
		ERES_RESOURCES,
		ERES_MATERIAL,
		ERES_GAMEOBJECT_PROPS,

		ERES_GAMEOBJECT_IDLE,
		ERES_GAMEOBJECT_HOVER,
		ERES_GAMEOBJECT_PRESSED,
		ERES_GAMEOBJECT_SELECTED,

		ERES_EXIT_IDLE,
		ERES_EXIT_HOVER,
		ERES_EXIT_PRESSED,

		ERES_MIN_IDLE,
		ERES_MIN_HOVER,
		ERES_MIN_PRESSED,

		ERES_MAX_IDLE,
		ERES_MAX_HOVER,
		ERES_MAX_PRESSED,

		ERES_BTN1_IDLE,
		ERES_BTN1_HOVER,
		ERES_BTN1_PRESSED,

		ERES_UP_IDLE,
		ERES_UP_HOVER,
		ERES_UP_PRESSED,

		ERES_DOWN_IDLE,
		ERES_DOWN_HOVER,
		ERES_DOWN_PRESSED,

		ERES_NEW_IDLE,
		ERES_NEW_HOVER,
		ERES_NEW_PRESSED,

		ERES_SLIDER,
		ERES_SLIDER_POINT,
		ERES_SLIDER_POINT_HOVER,
		ERES_SLIDER_POINT_PRESSED,
		ERES_SLIDER_MAX,
		ERES_SLIDER_MIN,

		ERES_TOTAL
	};

	class RZ_UTILITY_EXPORT Editor
	{
	public:
		void StartEditor(Gfx::GfxCore* gfx, ResourceManager* res, EventManager* evnt, Scene* scene);
		void UpdateEditor(float deltaTime);
		void RenderEditor();
		ResourceHandle GetUIResource(EDITOR_RESOURCE edtResource);

		void ChangeActiveGameObject(WeakPtr<GameObject> gameObj, UI_Button* uiButton);

		static ResourceHandle LoadFile(char* instruction, char* name, ResourceManager* res);

		UI_Canvas* GetCanvas()
		{
			return &m_CanvasPantalla;
		};

		WeakPtr<GameObject> m_ActiveGameObject;
		Editor_SceneGraph* m_SceneGraph;
		Editor_GameObjectProps* m_GameObjectProps;

		float planetScale = 0.0f;
		float planetDist = 0.0f;
		float realDist = 0.0f;

	private:
		Vector<ResourceHandle> m_Resources;

		void LoadResouces(ResourceManager* res);
		void UnloadResources(ResourceManager* res);

		Scene* m_CurrentScene;
		UI_Canvas m_CanvasPantalla;

		Gfx::GfxCore* m_gfx;
		ResourceManager* m_res;
		EventManager* m_evnt;

		UI_Button* m_ActiveGameObjectUI = NULL;

		int m_fps = 0;
		float m_second = 0.0f;

		WeakPtr<UI_Element> FPSText;
		WeakPtr<UI_Element> PlanetScale;
		WeakPtr<UI_Element> PlanetDist;
		WeakPtr<UI_Element> RealDist;
	};
};