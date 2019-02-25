#pragma once
#include <RZUtilityPCH.h>
#include "RZRay.h"

namespace rczEngine
{

	Ray::Ray() noexcept : m_StartPoint(eInit::None), m_Direction(eInit::None){}

	Ray::Ray(Vector3 start, Vector3 direction) noexcept : m_StartPoint(start), m_Direction(direction) {}

	Ray::Ray(float startx, float starty, float startz, float directionx, float directiony, float directionz) noexcept :
		m_StartPoint(startx, starty, startz), 
		m_Direction(directionx, directiony, directionz) {}
}