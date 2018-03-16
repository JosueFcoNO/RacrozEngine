#pragma once
#include <RZUtilityPCH.h>
#include "RZDegree.h"

namespace rczEngine
{
	void Degree::UnwindDegree(float F)
	{
		m_Value = Math::UnwindDegree(F);
	};

	Radian Degree::ValueRadian() {

		return Math::DegreesToRad(m_Value);
	};

	Degree::Degree(float F)
	{
		m_Value = F;
	};
}