#pragma once
#include "CbnUtilitiesPCH.h"
#include "RZColor.h"

namespace rczEngine
{
	void Color::AddColor(const Color& c) noexcept {	*this += c;	}

	void Color::RemoveColor(const Color& c) noexcept
	{
		m_x = (Math::Max(m_x - c.m_x, 0.0f));
		m_y = (Math::Max(m_y - c.m_y, 0.0f));
		m_z = (Math::Max(m_z - c.m_z, 0.0f));
		m_w = (Math::Max(m_w - c.m_w, 0.0f));
	}
}