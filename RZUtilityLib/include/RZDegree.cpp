#include "RZUtilityPCH.h"

namespace rczEngine
{

	Radian Degree::ValueRadian() const
	{
		return Math::DegreesToRad(m_Value);
	}

	void Degree::UnwindDegree(float F)
	{
		m_Value = Math::UnwindDegree(F);
	}
}