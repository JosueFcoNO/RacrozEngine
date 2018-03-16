#pragma once

namespace rczEngine
{
	template <typename t>
	class UI_Button_Variable : public UI_Button_Action
	{
	public:
		void InitButtonVar(t* variable, t delta, t minValue, t maxValue, bool increase, WeakPtr<UI_Text> text)
		{
			m_Variable = variable;
			m_Delta = delta;
			m_Increase = increase;
			m_Max = maxValue;
			m_Min = minValue;
			m_Text = text;
		}

		///Do an action, gets called by the UI_Button on clicked
		virtual void DoAction()
		{
			if (m_Increase)
			{
				*m_Variable += m_Delta;
				*m_Variable = Math::Min(*m_Variable, m_Max);
			}
			else
			{
				*m_Variable += -m_Delta;
				*m_Variable = Math::Max(*m_Variable, m_Min);

			}

			float varf = int32(*m_Variable) - *m_Variable;

			if (varf)
				m_Text.lock()->SetText(TextTool::WFloatTo(*m_Variable));
			else
				m_Text.lock()->SetText(TextTool::WIntTo(*m_Variable));

		};

	private:
		t* m_Variable = NULL;
		t m_Delta = 1;
		t m_Min = 1;
		t m_Max = 100;
		WeakPtr<UI_Text> m_Text;
		bool m_Increase = true;
	};
};