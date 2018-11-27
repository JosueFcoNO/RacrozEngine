#pragma once

namespace rczEngine
{
	class RZ_EXP Rect2D
	{
	public:
		Rect2D();
		Rect2D(Vector2 p1, Vector2 p2);

		void Set(Vector2 p1, Vector2 p2);

		Vector2 m_P1;
		Vector2 m_P2;
	};
}