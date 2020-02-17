#pragma once
#include "CbnUtilitiesPCH.h"
#include "RZRect3D.h"

namespace rczEngine
{
	//////////////////////////////////////////////////
	//////RECT 3
	//////////////////////////////////////////////////

	Rect3D::Rect3D() noexcept : m_P1(eInit::None), m_P2(eInit::None) {}

	Rect3D::Rect3D(Vector3 p1, Vector3 p2) noexcept : m_P1(p1), m_P2(p2)	{}

	void Rect3D::Set(Vector3 p1, Vector3 p2) noexcept
	{
		m_P1 = p1;
		m_P2 = p2;
	}
}