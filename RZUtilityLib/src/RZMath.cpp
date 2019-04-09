#pragma once
#include <RZUtilityPCH.h>
#include "RZMath.h"

namespace rczEngine
{
	Vector2 Math::PolarToPos2D(Vector3 polar) noexcept
	{
		return Vector2(
			atan2(polar.m_x, polar.m_z) / (PI2)+0.5f, //X
			asin(-polar.m_y) / PI + 0.5f			  //Y
		);
	};

	Vector3 Math::PosToPolar2D(Vector2 pos) noexcept
	{
		return Vector3(
			cos(PI*(0.5f - pos.m_y))*sin(PI2*(pos.m_x - 0.5f)),		//X 
			sin(PI*(0.5f - pos.m_y)),								//Y
			cos(PI*(0.5f - pos.m_y))*cos(PI2*(pos.m_x - 0.5f)));	//Z
	}

	Vector3 Math::PolarToPos3D(Vector3 polar) noexcept
	{
		return Vector3(
			polar.m_x * sin(polar.m_y) * cos(polar.m_z),  //X
			polar.m_x * sin(polar.m_y) * sin(polar.m_z),  //Y
			polar.m_x * polar.m_y);						  //Z
	}

	Vector3 Math::cubify(const Vector3 & s) noexcept
	{
		const float isqrt2 = 0.70710676908493042;

		const float xx2 = s.m_x * s.m_x * 2.0f;
		const float yy2 = s.m_y * s.m_y * 2.0f;

		Vector2 v(xx2 - yy2, yy2 - xx2);

		float ii = v.m_y - 3.0f;
		ii *= ii;

		const float isqrt = -sqrt(ii - 12.0f * xx2) + 3.0f;

		v.m_x = sqrt(fmax(v.m_x + isqrt, 0.0f));
		v.m_y = sqrt(fmax(v.m_y + isqrt, 0.0f));

		v *= isqrt2;

		return Vector3(v.m_x, v.m_y, 1.0f) * Sign(s);
	}

	Vector3 Math::SphereToCubePoint(const Vector3 & spherePoint)
	{
		Vector3 f = (spherePoint);
		f.m_x = fabs(f.m_x);
		f.m_y = fabs(f.m_y);
		f.m_z = fabs(f.m_z);

		const bool a = f.m_y >= f.m_x && f.m_y >= f.m_z;
		const bool b = f.m_x >= f.m_z;

		f = spherePoint;

		auto temp = 0.0f;

		if (a)
		{
			temp = f.m_y;
			f.m_y = f.m_z;
			f.m_z = temp;

			f = cubify(f);

			temp = f.m_y;
			f.m_y = f.m_z;
			f.m_z = temp;
		}
		else if (b)
		{
			float xx = f.m_x;
			float yy = f.m_y;
			float zz = f.m_z;

			f.m_x = yy;
			f.m_y = zz;
			f.m_z = xx;

			f = cubify(f);

			xx = f.m_x;
			yy = f.m_y;
			zz = f.m_z;

			f.m_x = zz;
			f.m_y = xx;
			f.m_z = yy;
		}
		else
		{
			f = cubify(f);
		}

		return f;
	}

	Vector3 Math::CubeToSphere(const Vector3 & cubepoint) noexcept
	{
		const float x2 = pow(cubepoint.m_x, 2);
		const float y2 = pow(cubepoint.m_y, 2);
		const float z2 = pow(cubepoint.m_z, 2);

		Vector3 posFinal;

		posFinal.m_x = cubepoint.m_x * sqrt(1.0f - y2 / 2.0f -
			z2 / 2.0f +
			y2 * z2 / 3.0f);

		posFinal.m_y = cubepoint.m_y * sqrt(1.0f - z2 / 2.0f -
			x2 / 2.0f +
			x2 * z2 / 3.0f);

		posFinal.m_z = cubepoint.m_z * sqrt(1.0f - x2 / 2.0f -
			y2 / 2.0f +
			y2 * x2 / 3.0f);

		return posFinal;
	}

	float Math::UnwindDegree(float f) noexcept
	{
		///Check if the value exceeds 360°, else return f
		if (f > 360)
		{
			///Create a Temp and give it f
			auto Temp = f;
			///Divide the Temp between 360° to get how many Loops it does
			auto Loops = gsl::narrow_cast<int32>(Temp / 360);
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

	float Math::UnwindRadian(float f) noexcept
	{
		///Check if the value exceeds 2PI, else return f
		if (f > 2 * PI)
		{
			///Create a Temp float and give it f
			float Temp = f;
			///Divide the Temp between 2Pi to get how many loops it does
			const int32 Loops = gsl::narrow_cast<int32>(Temp / (2 * PI));
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