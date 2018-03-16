#pragma once
#include <RZUtilityPCH.h>
#include "RZRect2D.h"

namespace rczEngine
{
	//////////////////////////////////////////////////
	//////RECT 2
	//////////////////////////////////////////////////

	Rect2D::Rect2D() : m_P1(INIT_NONE), m_P2(INIT_NONE)
	{
		m_P1.Set(0, 0);
		m_P2.Set(1, 1);
	}

	Rect2D::Rect2D(Vector2 p1, Vector2 p2) : m_P1(INIT_NONE), m_P2(INIT_NONE)
	{
		m_P1 = p1;
		m_P2 = p2;
	}

	void Rect2D::Set(Vector2 p1, Vector2 p2)
	{
		m_P1 = p1;
		m_P2 = p2;
	}
}