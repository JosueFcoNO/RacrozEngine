#pragma once

namespace rczEngine
{
	typedef Vector<Editor_GameObject*> Editor_GameObjectVector;

	class RZ_UTILITY_EXPORT Editor_SceneGraph : public Editor_Element
	{
	public:
		void InitSceneGraphEditor(Scene* sceneGraph, Editor* editor);
		void UpdateScene();

		void ActorListDown() 
		{ 
			++m_ActorOffset; 

			if (m_ActorOffset == m_Scene->m_SceneActorMap.size())
				m_ActorOffset = m_Scene->m_SceneActorMap.size() - 1;
		};
		void ActorListUp() 
		{ 
			--m_ActorOffset;

			if (m_ActorOffset < 0) 
				m_ActorOffset = 0; 
		};

		void CreateActor()
		{
			
		};

		virtual void HideElement()
		{
			m_SceneGraphBox.lock()->SetLocalPosition(m_HidePos.m_x, m_HidePos.m_y);
			m_ButtonMax.lock()->SetLocalPosition(-33, 0);
			m_ButtonMin.lock()->SetLocalPosition(-33, 2000);

		};

		virtual void ShowElement() 
		{
			m_SceneGraphBox.lock()->SetLocalPosition(m_ShowPos.m_x, m_ShowPos.m_y);
			m_ButtonMin.lock()->SetLocalPosition(-33, 0);
			m_ButtonMax.lock()->SetLocalPosition(-33, 2000);

		};

	private:
		Editor* m_Editor;
		Scene*	m_Scene;

		Editor_GameObjectVector m_GameObjects;

		WeakPtr<UI_Element> m_SceneGraphBox;
		WeakPtr<UI_Element> m_ButtonMax;
		WeakPtr<UI_Element> m_ButtonMin;
		WeakPtr<UI_Element> TextSceneGraph;

		int32 m_ActorOffset = 0;

		Vector2 m_ShowPos;
		Vector2 m_HidePos;
	};
};
