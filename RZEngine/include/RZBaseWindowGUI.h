#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT GUIBaseWindow
	{
	public:
		virtual void InitWindow() = 0;
		virtual void RenderWindow() = 0;
	};
}