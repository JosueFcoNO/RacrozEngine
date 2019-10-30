#pragma once
#include <RZUtilityPCH.h>
#include "RZRadian.h"

namespace rczEngine
{
	Radian::Radian(float F) noexcept
	{
		m_Value = F;
	};

	void Radian::UnwindRadian(float f)
	{
		m_Value = Math::UnwindRadian(f);
	};

	Degree Radian::ValueDegree() const noexcept
	{
		return Math::RadiansToDegrees(m_Value);
	};

}