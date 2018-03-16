#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Editor_Component
	{
	public:
		void InitComponentSelector(int32 x, int32 y, Editor* editor, WeakCmpPtr component);

	private:
		WeakPtr<UI_Button> m_Button;
		WeakPtr<UI_Text> m_ButtonText;
	};
};