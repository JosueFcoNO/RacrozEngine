#pragma once
#include "CbnUtilitiesPCH.h"
#include "RZLine.h"

namespace rczEngine
{
	bool Line::TestPoint(const Vector3 & point)
	{
		//PointEnLinea = PointOne + U * (PointOne - PointTwo);

		//0 = [pointDado - (PointEnLinea)] dot (PointTwo - PointOne)
		//0 = [pointDado - (PointOne + T*(PointTwo - PointOne))] dot (PointTwo - PointOne)
		//0 = [pointDado - ((x1, y1, z1) + T*(x2-x1, y2-y1, z2-z1))] dot (x2-x1, y2-y1, z2-z1)
		//0 = [(x3, y3, z3) - ((x1, y1, z1) + T*(x2-x1, y2-y1, z2-z1))] dot (x2-x1, y2-y1, z2-z1)
		//0 = [(x3, y3, z3) - ((x1, y1, z1) + (x2T-x1T, y2T-y1T, z2T-z1T))] dot (x2-x1, y2-y1, z2-z1)
		//0 = [(x3, y3, z3) - ((x1+x2T-x1T, y1+y2T-y1T, z1+z2T-z1T))] dot (x2-x1, y2-y1, z2-z1)
		//0 = (x3-x1+x2T-x1T, y3-y1+y2T-y1T, z3-z1+z2T-z1T) dot (x2-x1, y2-y1, z2-z1)
		//x = (x3-x1+x2T-x1T)(x2-x1) = x2x3 - x1x3 - x1x2 + x1x1 + x2x2T - x1x2T - x1x2T + x1x1T

		return false;
	}

	bool Line::IntersectionSphere(const Sphere & sphere)
	{
		return false;
	}
}