#pragma once

namespace rczEngine
{
	class GenerateHeightMap : public UI_Button_Action
	{
	public:
		void InitGenHeightMap(SpaceManager* space)
		{
			m_Space = space;
		};

		virtual void DoAction()
		{
			m_Space->RegenerateTerrains();
		};

	private:
		SpaceManager* m_Space = NULL;
	};

	class RZ_UTILITY_EXPORT Editor_GameObjectProps : public Editor_Element
	{
	public:
		void InitGameObjectProps(Scene* sceneGraph, Editor* editor);
		void ChangedObject();
		void ChangeSelectedComponent(WeakCmpPtr component) {};
		void UpdatePoint(Vector2 polar);
		void CreateVariableChanger();

		void InitPlanet(SpaceManager* space);

		virtual void HideElement()
		{
			m_GameObjectPropBox.lock()->SetLocalPosition(m_HidePos.m_x, m_HidePos.m_y);
			m_ButtonMax.lock()->SetLocalPosition(235, -5);
			m_ButtonMin.lock()->SetLocalPosition(235, 2000);

		};
		virtual void ShowElement()
		{
			m_GameObjectPropBox.lock()->SetLocalPosition(m_ShowPos.m_x, m_ShowPos.m_y);
			m_ButtonMin.lock()->SetLocalPosition(235, -5);
			m_ButtonMax.lock()->SetLocalPosition(235, 2000);

		};

		WeakPtr<UI_Element> m_GameObjectPropBox;

		SpaceManager* spaceMng;

	private:
		Editor* m_Editor;
		Scene*	m_Scene;

		Editor_VarChanger<int>* Octaves;
		Editor_VarChanger<int>* Scale;
		Editor_VarChanger<int>* VScale;

		Editor_VarChanger<float>* Persistance;

		GenerateHeightMap* GenButton = NULL;
		WeakPtr<UI_Element> m_ButtonGenerator;


		WeakPtr<UI_Element> m_ButtonMax;
		WeakPtr<UI_Element> m_ButtonMin;
		WeakPtr<UI_Element> m_TextGameObjectProps;
		WeakPtr<UI_Element> m_TextGameObjectSelected;
		WeakPtr<UI_Element> m_Point;

		Vector<Editor_Component*> m_Components;

		Material* m_TerrainMaterial;
		ResourceHandle* m_HeightMapTexture;

		Vector2 m_ShowPos;
		Vector2 m_HidePos;
	};
};