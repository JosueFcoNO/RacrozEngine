#pragma once


namespace rczEngine
{
	class RZ_UTILITY_EXPORT Editor_Element
	{
	public:
		virtual void HideElement() = 0;
		virtual void ShowElement() = 0;
	};

	class RZ_UTILITY_EXPORT Editor_MinButtonAction : public UI_Button_Action
	{
	public:
		Editor_Element* m_Element = NULL;

		virtual void DoAction()
		{
			m_Element->HideElement();
		};
	};

	class RZ_UTILITY_EXPORT Editor_MaxButtonAction : public UI_Button_Action
	{
	public:
		Editor_Element* m_Element = NULL;

		virtual void DoAction()
		{
			m_Element->ShowElement();
		};
	};
}