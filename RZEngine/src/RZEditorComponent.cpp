#include <RZEnginePCH.h>

namespace rczEngine
{
	void Editor_Component::InitComponentSelector(int32 x, int32 y, Editor * editor, WeakCmpPtr component)
	{
		auto res = editor->GetUIResource(ERES_BTN1_IDLE);
		m_Button = editor->GetCanvas()->CreateUI_Button("component", res, res+1, res+2, NULL, x, y, 2, 143, 33);
		editor->m_GameObjectProps->m_GameObjectPropBox.lock()->AddChildren(m_Button);
		m_ButtonText = editor->GetCanvas()->CreateUI_Text(L"Transform", "textComponent", 5, 5, 2, 16);
		m_Button.lock()->AddChildren(m_ButtonText);
	}
}