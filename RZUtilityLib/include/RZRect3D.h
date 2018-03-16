#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Rect3D
	{
	public:
		Rect3D();
		Rect3D(Vector3 p1, Vector3 p2);

		void Set(Vector3 p1, Vector3 p2);

		Vector3 m_P1, m_P2;

	};
}