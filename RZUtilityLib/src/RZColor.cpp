#pragma once
#include <RZUtilityPCH.h>
#include "RZColor.h"

namespace rczEngine
{

	void Color::SetColor(float R, float G, float B, float A) noexcept
	{
		m_x = R;
		m_y = G;
		m_z = B;
		m_w = A;
	}

	void Color::AddColor(const Color& c) noexcept {	*this += c;	}

	void Color::RemoveColor(const Color& c) noexcept
	{
		m_x = (Math::Max(m_x - c.m_x, 0.0f));
		m_y = (Math::Max(m_y - c.m_y, 0.0f));
		m_z = (Math::Max(m_z - c.m_z, 0.0f));
		m_w = (Math::Max(m_w - c.m_w, 0.0f));
	}

	Color Color::GetLineal() const noexcept
	{
		Color newColor;

#pragma omp for
		for (int i = 0; i < 4; ++i)
		{
			newColor.m_elements[i] = Math::Pow(newColor.m_elements[i], 2.2f);
		};

		return newColor;
	}
	Color Color::GetSRGB() const noexcept
	{
		Color newColor;

#pragma omp for
		for (int i = 0; i < 4; ++i)
		{
			newColor.m_elements[i] = Math::Pow(m_elements[i], 1.0f / 2.2f);
		};

		return newColor;
	};
}