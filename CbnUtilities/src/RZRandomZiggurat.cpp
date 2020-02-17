#include "CbnUtilitiesPCH.h"
#include "..\include\RZRandomZiggurat.h"

namespace rczEngine
{
	uint32 RandomZiggurat::GetRandomNumber() noexcept
	{
		return uint32();
	}

	float RandomZiggurat::GetRandomNumberN() noexcept
	{
		return 0.0f;
	}

	float RandomZiggurat::TestX(float x) noexcept
	{
		return Math::Exp(-(Math::Square(x) / 2));
	}
}