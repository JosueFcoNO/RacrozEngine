#pragma once

namespace rczEngine
{
	class RZ_EXP Rect3D
	{
	public:
		Rect3D() noexcept;
		Rect3D(Vector3 p1, Vector3 p2) noexcept;

		void Set(Vector3 p1, Vector3 p2) noexcept;

		Vector3 m_P1, m_P2;

	};
}