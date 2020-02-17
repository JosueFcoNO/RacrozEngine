#pragma once
#include "CbnUtilitiesPCH.h"
#include "RZMath.h"

namespace rczEngine
{
	Vector3 Math::PosToPolar3D(Vector3 pos) noexcept
	{
		Vector3 ret;
		ret.m_x = pos.Magnitude();
		ret.m_y = aCos(pos.m_z / ret.m_x);
		ret.m_z = aTan2(pos.m_y, pos.m_x);

		return ret;
	};

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

	Vector3 Math::SphereToCubePoint(const Vector3 & spherePoint) noexcept
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

	Vector2 Math::Sign(Vector2 x) noexcept
	{
		Vector2 ret;
		ret.m_x = (x.m_x >= 0) ? 1.0f : -1.0f;
		ret.m_y = (x.m_y >= 0) ? 1.0f : -1.0f;

		return ret;
	}

	Vector2I Math::Sign(Vector2I x) noexcept
	{
		Vector2I ret;
		ret.m.x = (x.m.x >= 0) ? 1 : -1;
		ret.m.y = (x.m.y >= 0) ? 1 : -1;

		return ret;
	}

	Vector3 Math::Sign(Vector3 x) noexcept
	{
		Vector3 ret;
		ret.m_x = (x.m_x >= 0) ? 1.0f : -1.0f;
		ret.m_y = (x.m_y >= 0) ? 1.0f : -1.0f;
		ret.m_z = (x.m_z >= 0) ? 1.0f : -1.0f;

		return ret;
	}

	Vector4 Math::Sign(Vector4 x) noexcept
	{
		Vector4 ret(eInit::None);
		ret.m_x = (x.m_x >= 0) ? 1.0f : -1.0f;
		ret.m_y = (x.m_y >= 0) ? 1.0f : -1.0f;
		ret.m_z = (x.m_z >= 0) ? 1.0f : -1.0f;
		ret.m_w = (x.m_w >= 0) ? 1.0f : -1.0f;

		return ret;
	};

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

	////////////////////////////
	/// Fast Trigonometry
	////////////////////////////

	float TrigFast::FastSin0(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < 0 || angle > HALFPI)
		{
			return sinf(angle);
		}
		else
		{
			auto sin = 1.0f - 0.16605f * Math::Pow(angle, 2) + 0.00761f * Math::Pow(angle, 4);
			sin *= angle;

			return sin;
		}
	}

	float TrigFast::FastSin1(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < 0 || angle > HALFPI)
		{
			return sinf(angle);
		}
		else
		{
			auto sin = 1.0f -
				0.1666666664f*Math::Pow(angle, 2) +
				0.0083333315f*Math::Pow(angle, 4) -
				0.0001984090f*Math::Pow(angle, 6) +
				0.0000027526f*Math::Pow(angle, 8) -
				0.0000000239f*Math::Pow(angle, 10);

			sin *= angle;

			return sin;
		}
	}

	float TrigFast::FastCos0(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < 0 || angle > HALFPI)
		{
			return cosf(angle);
		}
		else
		{
			float cos = 0;

			for (float i = 0; i < 3; i++)
			{
				cos += (Math::Pow(-1, i) / Math::Factorial(2 * i))*(Math::Pow(angle, (2 * i)));
			}

			return cos;
		}

	}

	float TrigFast::FastCos1(float angle) noexcept
	{
		float cos = 0;

		///if the angle is not on the accepted range
		if (angle < 0 || angle > HALFPI)
		{
			return cosf(angle);
		}
		else
		{
			cos = 1.0f -
				0.4999999963f* Math::Pow(angle, 2) +
				0.0416666418f*Math::Pow(angle, 4) -
				0.0013888397f*Math::Pow(angle, 6) +
				0.0000247609f*Math::Pow(angle, 8) -
				0.0000002605f*Math::Pow(angle, 10);

			return cos;
		}

	}

	float TrigFast::FastTan0(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < 0 || angle > HALFPI / 2)
		{
			return tanf(angle);
		}
		else
		{
			auto tan = 1 + 0.31755f*Math::Pow(angle, 2) + 0.20330f*Math::Pow(angle, 4);
			tan *= angle;

			return tan;
		}
	}

	float TrigFast::FastTan1(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < 0 || angle > HALFPI)
		{
			return tanf(angle);
		}
		else
		{
			auto tan = 1.0f +
				0.3333314036f*Math::Pow(angle, 2) +
				0.1333923995f*Math::Pow(angle, 4) +
				0.0533740603f*Math::Pow(angle, 6) +
				0.0245650893f*Math::Pow(angle, 8) +
				0.0029005250f*Math::Pow(angle, 10) +
				0.0095168091f*Math::Pow(angle, 12);

			tan *= angle;

			return tan;
		}

	}

	float TrigFast::FastASin0(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < 0 || angle > 1)
		{
			return asinf(angle);
		}
		else
		{
			const auto asin = PI / 2 - Math::Sqrt(1.0f - angle)*(1.5707288f - 0.2121144f*angle
				+ 0.0742610f*Math::Pow(angle, 2) - 0.0187293f*Math::Pow(angle, 3));

			return asin;
		}
	}

	float TrigFast::FastASin1(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < 0 || angle > 1)
		{
			return asinf(angle);
		}
		else
		{
			const auto asin = HALFPI -
				Math::Sqrt(1 - angle)*(1.5707963050f - 0.2145988016f*angle +
					0.0889789874f* Math::Pow(angle, 2) -
					0.0501743046f*Math::Pow(angle, 3) +
					0.0308918810f*Math::Pow(angle, 4) -
					0.01708812556f*Math::Pow(angle, 5) +
					0.0066700901f*Math::Pow(angle, 6) -
					0.0012624911f*Math::Pow(angle, 7));

			return asin;
		}

	}

	float TrigFast::FastACos0(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < 0 || angle > 1)
		{
			return acosf(angle);
		}
		else
		{
			const auto acos = PI / 2 - FastASin0(angle);

			return acos;
		}

	}

	float TrigFast::FastACos1(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < 0 || angle > 1)
		{
			return acosf(angle);
		}
		else
		{
			const auto acos = PI / 2 - FastASin1(angle);

			return acos;
		}

	}

	float TrigFast::FastATan0(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < -1 || angle > 1)
		{
			return atanf(angle);
		}
		else
		{
			const auto atan =
				0.9998660f*angle -
				0.3302995f*Math::Pow(angle, 3) +
				0.1801410f*Math::Pow(angle, 5) -
				0.0851330f*Math::Pow(angle, 7) +
				0.0208351f*Math::Pow(angle, 9);

			return atan;
		}

	}

	float TrigFast::FastATan1(float angle) noexcept
	{
		///if the angle is not on the accepted range
		if (angle < -1 || angle > 1)
		{
			return atanf(angle);
		}
		else
		{
			auto atan = 1.0f -
				0.3333314528f*Math::Pow(angle, 2) +
				0.1999355085f*Math::Pow(angle, 4) -
				0.1420889944f*Math::Pow(angle, 6) +
				0.1065626393f*Math::Pow(angle, 8) -
				0.0752896400f*Math::Pow(angle, 10) +
				0.0429096138f*Math::Pow(angle, 12) -
				0.0161657367f*Math::Pow(angle, 14) +
				0.0028662257f*Math::Pow(angle, 16);
			atan *= angle;

			return atan;
		}

	}

}