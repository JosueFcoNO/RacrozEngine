#pragma once

namespace rczEngine
{
	class RZ_EXP GUIBaseWindow
	{
	public:
		virtual void InitWindow() = 0;
		virtual void RenderWindow() = 0;
	};
}