#pragma once

namespace rczEngine
{
	class RZ_EXP Rect2Di
	{
	public:
		Rect2Di();
		Rect2Di(Vector2I p1, Vector2I p2);

		void Set(Vector2I p1, Vector2I p2);

		Vector2I m_P1, m_P2;

	};
}