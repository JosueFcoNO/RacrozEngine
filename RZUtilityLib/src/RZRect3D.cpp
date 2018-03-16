#pragma once
#include <RZUtilityPCH.h>
#include "RZRect3D.h"

namespace rczEngine
{
	//////////////////////////////////////////////////
	//////RECT 3
	//////////////////////////////////////////////////

	Rect3D::Rect3D() : m_P1(INIT_NONE), m_P2(INIT_NONE)
	{
		m_P1.Set(0, 0, 0);
		m_P2.Set(1, 1, 1);
	}

	Rect3D::Rect3D(Vector3 p1, Vector3 p2) : m_P1(INIT_NONE), m_P2(INIT_NONE)
	{
		m_P1 = p1;
		m_P2 = p2;
	}

	void Rect3D::Set(Vector3 p1, Vector3 p2)
	{
		m_P1 = p1;
		m_P2 = p2;
	}
}