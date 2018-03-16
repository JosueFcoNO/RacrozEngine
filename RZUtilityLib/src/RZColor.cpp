#pragma once
#include <RZUtilityPCH.h>
#include "RZColor.h"

namespace rczEngine
{

	Color::Color()
	{
		R = 0;
		G = 0;
		B = 0;
		A = 0;
	}

	Color::Color(uint8 r, uint8 g, uint8 b, uint8 a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}


	void Color::Set(uint8 r, uint8 g, uint8 b, uint8 a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	void Color::Add(Color c)
	{
		R = (uint8)Math::Min(c.R + R, 255);
		G = (uint8)Math::Min(c.G + G, 255);
		B = (uint8)Math::Min(c.B + B, 255);
		A = (uint8)Math::Min(c.A + A, 255);
	}

	void Color::Remove(Color c)
	{
		R = (uint8)Math::Max(R - c.R, 255);
		G = (uint8)Math::Max(G - c.G, 255);
		B = (uint8)Math::Max(B - c.B, 255);
		A = (uint8)Math::Max(A - c.A, 255);
	}
}