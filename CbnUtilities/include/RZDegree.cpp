#include "CbnUtilitiesPCH.h"

namespace rczEngine
{

	Radian Degree::ValueRadian() const noexcept
	{
		return Math::DegreesToRad(m_Value);
	}

	void Degree::UnwindDegree(float F) noexcept
	{
		m_Value = Math::UnwindDegree(F);
	}
}