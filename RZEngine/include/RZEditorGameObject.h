#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Editor_GameObjectButton : public UI_Button_Action
	{
	public:
		void InitGameObjectButton(Editor* editor, WeakPtr<GameObject> gameObject, UI_Button* uiGameobject)
		{
			m_Editor = editor;
			m_GameObject = gameObject;
			m_uiGameobject = uiGameobject;
		};

		virtual void DoAction()
		{
			m_Editor->ChangeActiveGameObject(m_GameObject, m_uiGameobject);
			m_uiGameobject->m_TextureIdle = m_Editor->GetUIResource(ERES_GAMEOBJECT_SELECTED);
		};

	private:
		Editor* m_Editor;
		WeakPtr<GameObject> m_GameObject;
		UI_Button* m_uiGameobject;
	};

	class RZ_UTILITY_EXPORT Editor_GameObject
	{
	public:
		void InitEditorGameObject(Editor* editor, StrPtr<GameObject> gameObj);
		void SetPos(int32 x, int32 y);

		WeakPtr<UI_Element> m_Button;

	private:
		WeakPtr<GameObject> m_GameObject;
		Editor_GameObjectButton m_Action;
	};
}