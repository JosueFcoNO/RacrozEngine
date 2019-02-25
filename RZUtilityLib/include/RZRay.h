#pragma once

namespace rczEngine
{
	///Ray class
	class RZ_EXP Ray
	{
	public:
		Ray() noexcept;
		Ray(Vector3 start, Vector3 direction) noexcept;
		Ray(float startx, float starty, float startz, float directionx, float directiony, float directionz) noexcept;


		Vector3 m_StartPoint;
		Vector3 m_Direction;
	};
}