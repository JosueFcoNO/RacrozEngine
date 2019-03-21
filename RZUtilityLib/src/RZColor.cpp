#pragma once
#include <RZUtilityPCH.h>
#include "RZColor.h"

namespace rczEngine
{

	void Color::Set(float R, float G, float B, float A) noexcept
	{
		m_x = R;
		m_y = G;
		m_z = B;
		m_w = A;
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