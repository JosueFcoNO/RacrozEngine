#pragma once

namespace rczEngine
{
	class RZ_EXP GUIGameObject : GUIBaseWindow
	{
	public:
		virtual void InitWindow();
		virtual void RenderWindow();

		void SetNewGameObject(StrGameObjectPtr obj);
		
	private:
		char m_Name[50];
		StrGameObjectPtr m_ActiveObject;
	};
}