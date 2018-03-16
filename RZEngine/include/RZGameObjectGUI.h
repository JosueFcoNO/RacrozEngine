#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT GUIGameObject : GUIBaseWindow
	{
	public:
		virtual void InitWindow();
		virtual void RenderWindow();

		void SetNewGameObject(StrGameObjectPtr obj);
		
	private:
		char m_Name[20];
		StrGameObjectPtr m_ActiveObject;
	};
}