#pragma once
#include <RZUtilityPCH.h>
#include "RZRect2Di.h"

namespace rczEngine
{
	//////////////////////////////////////////////////
	//////RECT 2I
	//////////////////////////////////////////////////

	Rect2Di::Rect2Di() : m_P1(INIT_NONE), m_P2(INIT_NONE)
	{
		m_P1.Set(0, 0);
		m_P2.Set(1, 1);
	}

	Rect2Di::Rect2Di(Vector2I p1, Vector2I p2) : m_P1(INIT_NONE), m_P2(INIT_NONE)
	{
		m_P1 = p1;
		m_P2 = p2;
	}

	void Rect2Di::Set(Vector2I p1, Vector2I p2)
	{
		m_P1 = p1;
		m_P2 = p2;
	}
}