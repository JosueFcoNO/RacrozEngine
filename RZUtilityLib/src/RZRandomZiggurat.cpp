#include <RZUtilityPCH.h>
#include "..\include\RZRandomZiggurat.h"

namespace rczEngine
{
	uint32 RandomZiggurat::GetRandomNumber()
	{
		return uint32();
	}

	float RandomZiggurat::GetRandomNumberN()
	{
		return 0.0f;
	}

	float RandomZiggurat::TestX(float x)
	{
		return Math::Exp(-(Math::Square(x) / 2));
	}
}