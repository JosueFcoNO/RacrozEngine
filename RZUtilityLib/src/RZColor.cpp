#pragma once
#include <RZUtilityPCH.h>
#include "RZColor.h"

namespace rczEngine
{

	void Color::Set(uint8 r, uint8 g, uint8 b, uint8 a) noexcept
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	void Color::Add(Color c) noexcept
	{
		R = gsl::narrow_cast<uint8>(Math::Min(c.R + R, 255) );
		G = gsl::narrow_cast<uint8>(Math::Min(c.G + G, 255) );
		B = gsl::narrow_cast<uint8>(Math::Min(c.B + B, 255) );
		A = gsl::narrow_cast<uint8>(Math::Min(c.A + A, 255) );
	}

	void Color::Remove(Color c) noexcept
	{
		R = gsl::narrow_cast<uint8>(Math::Max(R - c.R, 255) );
		G = gsl::narrow_cast<uint8>(Math::Max(G - c.G, 255) );
		B = gsl::narrow_cast<uint8>(Math::Max(B - c.B, 255) );
		A = gsl::narrow_cast<uint8>(Math::Max(A - c.A, 255) );
	}
}