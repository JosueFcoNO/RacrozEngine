//#include <RZEnginePCH.h>

namespace rczEngine
{
	void Editor_GameObject::InitEditorGameObject(Editor* editor, StrPtr<GameObject> gameObj)
	{
		Editor_GameObjectButton* ptr = new Editor_GameObjectButton;

		m_GameObject = gameObj;
		m_Button = editor->GetCanvas()->CreateUI_Button(&gameObj->GetName()[0], editor->GetUIResource(ERES_GAMEOBJECT_IDLE), editor->GetUIResource(ERES_GAMEOBJECT_HOVER), editor->GetUIResource(ERES_GAMEOBJECT_PRESSED), ptr, 0, 0, 0, 252, 27);
		
		ptr->InitGameObjectButton(editor, gameObj, (UI_Button*)m_Button.lock().get());

		m_Button.lock()->AddChildren(editor->GetCanvas()->CreateUI_Text(TextTool::AnsiToUni(&gameObj->GetName()[0]), &(gameObj->GetName() + " text")[0], 10, 0, 30, 22));
	}

	void Editor_GameObject::SetPos(int32 x, int32 y)
	{
		m_Button.lock()->SetLocalPosition(x, y);
	}
}