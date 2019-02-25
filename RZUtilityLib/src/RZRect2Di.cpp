#pragma once
#include <RZUtilityPCH.h>
#include "RZRect2Di.h"

namespace rczEngine
{
	//////////////////////////////////////////////////
	//////RECT 2I
	//////////////////////////////////////////////////

	Rect2Di::Rect2Di() noexcept : m_P1(eInit::None), m_P2(eInit::None) {}

	Rect2Di::Rect2Di(Vector2I p1, Vector2I p2) noexcept : m_P1(p1), m_P2(p2) {}

	void Rect2Di::Set(Vector2I p1, Vector2I p2) noexcept
	{
		m_P1 = p1;
		m_P2 = p2;
	}
}