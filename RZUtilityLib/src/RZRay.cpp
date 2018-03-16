#pragma once
#include <RZUtilityPCH.h>
#include "RZRay.h"

namespace rczEngine
{

	Ray::Ray() : m_StartPoint(INIT_NONE), m_Direction(INIT_NONE)
	{
		m_StartPoint.Set(0, 0, 0);
		m_Direction.Set(1, 0, 0);
	}

	Ray::Ray(Vector3 start, Vector3 direction) : m_StartPoint(INIT_NONE), m_Direction(INIT_NONE)
	{
		m_StartPoint = start;
		m_Direction = direction;
	}

	Ray::Ray(float startx, float starty, float startz, float directionx, float directiony, float directionz) : m_StartPoint(INIT_NONE), m_Direction(INIT_NONE)
	{
		m_StartPoint.Set(startx, starty, startz);
		m_Direction.Set(directionx, directiony, directionz);
	}
}