#pragma once
#include <RZUtilityPCH.h>
#include "RZColor.h"

namespace rczEngine
{

	void Color::Set(float r, float g, float b, float a) noexcept
	{
		m_x = r;
		m_y = g;
		m_z = b;
		m_w = a;
	}

	void Color::AddColor(const Color& c) noexcept
	{
		m_x = (Math::Min(c.r + m_x, 1.0f));
		m_y = (Math::Min(c.g + m_y, 1.0f));
		m_z = (Math::Min(c.b + m_z, 1.0f));
		m_w = (Math::Min(c.a + m_w, 1.0f) );
	}

	void Color::RemoveColor(const Color& c) noexcept
	{
		m_x = (Math::Max(m_x - c.m_x, 1.0f));
		m_y = (Math::Max(m_y - c.m_y, 1.0f));
		m_z = (Math::Max(m_z - c.m_z, 1.0f));
		m_w = (Math::Max(m_w - c.m_w, 1.0f));
	}
}