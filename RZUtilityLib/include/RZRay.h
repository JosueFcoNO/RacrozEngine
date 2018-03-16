#pragma once

namespace rczEngine
{
	///Ray class
	class RZ_UTILITY_EXPORT Ray
	{
	public:
		Ray();
		Ray(Vector3 start, Vector3 direction);
		Ray(float startx, float starty, float startz, float directionx, float directiony, float directionz);


		Vector3 m_StartPoint;
		Vector3 m_Direction;
	};
}