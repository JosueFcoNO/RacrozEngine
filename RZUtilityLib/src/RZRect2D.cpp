#pragma once
#include <RZUtilityPCH.h>
#include "RZRect2D.h"

namespace rczEngine
{
	//////////////////////////////////////////////////
	//////RECT 2
	//////////////////////////////////////////////////

	Rect2D::Rect2D() noexcept : m_P1(eInit::None), m_P2(eInit::None){}

	Rect2D::Rect2D(Vector2 p1, Vector2 p2) noexcept : m_P1(p1), m_P2(p2){}

	void Rect2D::Set(Vector2 p1, Vector2 p2) noexcept
	{
		m_P1 = p1;
		m_P2 = p2;
	}
}