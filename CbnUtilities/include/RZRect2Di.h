#pragma once

namespace rczEngine
{
	class RZ_EXP Rect2Di
	{
	public:
		Rect2Di() noexcept;
		Rect2Di(Vector2I p1, Vector2I p2) noexcept;

		void Set(Vector2I p1, Vector2I p2) noexcept;

		Vector2I m_P1, m_P2;

	};
}