#pragma once
#include <RZUtilityPCH.h>
#include "RZMath.h"

namespace rczEngine
{
	float Math::UnwindDegree(float f) {
		///Check if the value exceeds 360°, else return f
		if (f > 360)
		{
			///Create a Temp and give it f
			float Temp = f;
			///Divide the Temp between 360° to get how many Loops it does
			int32 Loops = int32(Temp / 360);
			///substract Loops*360° to Temp, to eliminate the degrees from extra loops
			Temp -= Loops * 360;
			///Return the value in Temp
			return Temp;
		}
		else
		{
			return f;
		}
	};

	float Math::UnwindRadian(float f) {
		///Check if the value exceeds 2PI, else return f
		if (f > 2 * PI)
		{
			///Create a Temp float and give it f
			float Temp = f;
			///Divide the Temp between 2Pi to get how many loops it does
			int32 Loops = int32(Temp / (2 * PI));
			///Substract Loops*2PI to Temp, to eliminate the radians from extra loops
			Temp -= Loops * (2 * PI);
			///return the value in Temp
			return Temp;
		}
		else
		{
			return f;
		}
	};
}