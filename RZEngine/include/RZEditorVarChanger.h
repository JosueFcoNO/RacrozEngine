#pragma once

namespace rczEngine
{
	template <typename t>
	class Editor_VarChanger
	{
	public:
		void InitVarChanger(t* variable, char* variableName, int32 x, int32 y, t minValue, t maxValue, t delta, WeakPtr<UI_Element> parent, Editor* editor)
		{
			ResourceHandle Up = editor->GetUIResource(ERES_UP_IDLE);
			ResourceHandle Down = editor->GetUIResource(ERES_DOWN_IDLE);

			auto Text = editor->GetCanvas()->CreateUI_Text(TextTool::AnsiToUni(variableName), "GameObjectTextBox", x - 60, y - 30, 18, 20);
			parent.lock()->AddChildren(Text);

			float variableIsFloat = int32(*variable) - *variable;

			if (variableIsFloat)
				Text = editor->GetCanvas()->CreateUI_Text(TextTool::WFloatTo(*variable), "Text", x, y, 18, 20);
			else
				Text = editor->GetCanvas()->CreateUI_Text(TextTool::WIntTo(*variable), "Text", x, y, 18, 20);
			parent.lock()->AddChildren(Text);

			m_ButtonDown.InitButtonVar(variable, delta, minValue, maxValue, false, Text);
			m_ButtonUp.InitButtonVar(variable, delta, minValue, maxValue, true, Text);

			auto Button = editor->GetCanvas()->CreateUI_Button("ButtonLess", Down, Down + 1, Down + 2, &m_ButtonDown, x - 50, y, 2, 33, 34);
			parent.lock()->AddChildren(Button);

			Button = editor->GetCanvas()->CreateUI_Button("ButtonMore", Up, Up + 1, Up + 2, &m_ButtonUp, x + 50, y, 2, 33, 34);
			parent.lock()->AddChildren(Button);
		};

	private:
		UI_Button_Variable<t> m_ButtonDown;
		UI_Button_Variable<t> m_ButtonUp;

		t* m_Variable = NULL;
		t m_MinValue = 0;
		t m_MaxValue = 100;

	};
}