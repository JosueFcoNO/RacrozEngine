#pragma once

///Define PI, 2PI and HALFPI
#undef PI
#define PI (3.1415926535897932f)
#define PI2 2*(3.1415926535897932f)
#define HALFPI (3.1415926535897932f)/2

namespace rczEngine
{
	///General Math Functions Class
	class RZ_EXP Math
	{
	public:
		///Multiplication by 2 using bit shifts
		static FORCEINLINE constexpr int32 MulX2(int32 V) noexcept { return V << 1; };

		///Division by 2 using bit shifts
		static FORCEINLINE constexpr int32 DivX2(int32 V) noexcept { return V >> 1; };

		///Truncate float to int
		static FORCEINLINE int32 Truncate(float F) noexcept { return gsl::narrow_cast<int32>(F); };
		///Truncate float to float as int
		static FORCEINLINE float TruncateF(float F) noexcept { return std::floorf(F); };
		///Floor a float
		static FORCEINLINE float Floor(float F) noexcept { return TruncateF(floorf(F)); };
		///Round a float
		static FORCEINLINE float Round(float F) noexcept { return Floor(F + 0.5f); };
		///Ceil a float
		static FORCEINLINE float Ceil(float F) noexcept { return TruncateF(F) + 1.0f; };

		static Vector3 PosToPolar3D(Vector3 pos) noexcept
		{
			Vector3 ret;
			ret.m_x = pos.Magnitude();
			ret.m_y = aCos(pos.m_z / ret.m_x);
			ret.m_z = aTan2(pos.m_y, pos.m_x);

			return ret;
		};

		static Vector2 PolarToPos2D(Vector3 polar)
		{
			return Vector2(
				atan2(polar.m_x, polar.m_z) / (PI2)+0.5f, //X
				asin(-polar.m_y) / PI + 0.5f			  //Y
			);
		};

		static Vector3 PosToPolar2D(Vector2 pos) noexcept
		{
			return Vector3(
				cos(PI*(0.5f - pos.m_y))*sin(PI2*(pos.m_x - 0.5f)),		//X 
				sin(PI*(0.5f - pos.m_y)),								//Y
				cos(PI*(0.5f - pos.m_y))*cos(PI2*(pos.m_x - 0.5f)));	//Z
		}

		static Vector3 PolarToPos3D(Vector3 polar) noexcept
		{
			return Vector3(
				polar.m_x * sin(polar.m_y) * cos(polar.m_z),  //X
				polar.m_x * sin(polar.m_y) * sin(polar.m_z),  //Y
				polar.m_x * polar.m_y);						  //Z
		};

		static Vector3 cubify(const Vector3& s)
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

		static Vector3 SphereToCubePoint(const Vector3& spherePoint)
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

		static Vector3 CubeToSphere(const Vector3 &cubepoint) noexcept
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

		///Elevate a float to ipow
		template<class c>
		static FORCEINLINE float Pow(c V, float ipow) noexcept { return std::pow(V, ipow); };
		///Float module
		static FORCEINLINE float fMod(float X, float Y) noexcept { return std::fmodf(X, Y); };
		///Float absolute value
		static FORCEINLINE float fAbs(float V) noexcept { return std::fabsf(V); };
		///Float Logaritmic
		static FORCEINLINE float Log(float V) noexcept { return std::log(V); };

		///Float Factorial
		static FORCEINLINE constexpr float Factorial(float N) noexcept
		{
			float Temp = 1;

			for (int i = 1; i <= N; i++)
			{
				Temp *= i;
			}

			return Temp;
		};

		///Exp of a float
		static FORCEINLINE float Exp(float V) noexcept { return std::exp(V); };
		///Checks if it's not a number
		static FORCEINLINE bool IsNan(float A) noexcept { return std::isnan<float>(A); }
		///Degrees To Radian
		static FORCEINLINE constexpr float DegreesToRad(float F) noexcept { return F*(PI / 180.0f); };
		///Radian to Degrees
		static FORCEINLINE constexpr float RadiansToDegrees(float F) noexcept { return F*(180.0f / PI); };

		///Trig Sin
		static FORCEINLINE float Sin(float angle) noexcept { return sinf(angle); };
		///Trig Cos
		static FORCEINLINE float Cos(float angle) noexcept { return cosf(angle); };
		///Trig Tan
		static FORCEINLINE float Tan(float angle) noexcept { return tanf(angle); };

		///Trig arcSin
		static FORCEINLINE float aSin(float rad) noexcept { return asinf(rad); };
		///Trig arccos
		static FORCEINLINE float aCos(float rad) noexcept { return acosf(rad); };
		///Trig arctan
		static FORCEINLINE float aTan(float rad) noexcept { return atanf(rad); };

		///Square root of a float
		static FORCEINLINE float Sqrt(float F) noexcept { return std::sqrtf(F);	};

		///Fast Sin function using taylor series, fastest
		static float FastSin0(float angle) noexcept
		{
			float sin = 0;

			///if the angle is not on the accepted range
			if (angle < 0 || angle > HALFPI)
			{
				return sinf(angle);
			}
			else
			{
				sin = 1 - 0.16605f*angle*angle + 0.00761f*angle*angle*angle*angle;
				sin *= angle;

				return sin;
			}
		}

		///Fast Sin function using taylor series, most accurate
		static float FastSin1(float angle) noexcept
		{
			float sin = 0;

			///if the angle is not on the accepted range
			if (angle < 0 || angle > HALFPI)
			{
				return sinf(angle);
			}
			else
			{
				sin = 1 - 0.1666666664f*Pow(angle, 2) + 0.0083333315f*Pow(angle, 4) - 0.0001984090f*Pow(angle, 6) + 0.0000027526f*Pow(angle, 8) - 0.0000000239f*Pow(angle, 10);
				sin *= angle;

				return sin;
			}

		}

		///Fast Cos function using taylor series, fastest
		static FORCEINLINE float FastCos0(float angle) noexcept
		{
			float cos = 0;

			///if the angle is not on the accepted range
			if (angle < 0 || angle > HALFPI)
			{
				return cosf(angle);
			}
			else
			{
				for (float i = 0; i < 3; i++)
				{
					cos += (powf(-1, i) / rczEngine::Math::Factorial(2 * i))*(powf(angle, (2 * i)));
				}

				return cos;
			}

		}

		///Fast Sin function using taylor series, most accurate
		static FORCEINLINE float FastCos1(float angle) noexcept
		{
			float cos = 0;

			///if the angle is not on the accepted range
			if (angle < 0 || angle > HALFPI)
			{
				return cosf(angle);
			}
			else
			{
				cos = 1 - 0.4999999963f* Pow(angle, 2) + 0.0416666418f*Pow(angle, 4) - 0.0013888397f* Pow(angle, 6) + 0.0000247609f*Pow(angle, 8) - 0.0000002605f*Pow(angle, 10);


				return cos;
			}

		}

		static FORCEINLINE constexpr float Sign(float x) noexcept	{ return (x >= 0.0f) ? 1.0f : -1.0f; };

		static FORCEINLINE constexpr int Sign(int x) noexcept { return (x >= 0) ? 1 : -1;	};

		static Vector2 Sign(Vector2 x) noexcept
		{
			Vector2 ret;
			ret.m_x = (x.m_x >= 0) ? 1.0f : -1.0f;
			ret.m_y = (x.m_y >= 0) ? 1.0f : -1.0f;

			return ret;
		};

		static Vector2I Sign(Vector2I x) noexcept
		{
			Vector2I ret;
			ret.m.x = (x.m.x >= 0) ? 1 : -1;
			ret.m.y = (x.m.y >= 0) ? 1 : -1;

			return ret;
		};

		static Vector3 Sign(Vector3 x) noexcept
		{
			Vector3 ret;
			ret.m_x = (x.m_x >= 0) ? 1.0f : -1.0f;
			ret.m_y = (x.m_y >= 0) ? 1.0f : -1.0f;
			ret.m_z = (x.m_z >= 0) ? 1.0f : -1.0f;

			return ret;
		};

		static Vector4 Sign(Vector4 x) noexcept
		{
			Vector4 ret(eInit::None);
			ret.m_x = (x.m_x >= 0) ? 1.0f : -1.0f;
			ret.m_y = (x.m_y >= 0) ? 1.0f : -1.0f;
			ret.m_z = (x.m_z >= 0) ? 1.0f : -1.0f;
			ret.m_w = (x.m_w >= 0) ? 1.0f : -1.0f;

			return ret;
		};

		///Fast Tan function using taylor series, fastest
		static FORCEINLINE float FastTan0(float angle) noexcept
		{
			float tan = 0;

			///if the angle is not on the accepted range
			if (angle < 0 || angle > HALFPI / 2)
			{
				return tanf(angle);
			}
			else
			{
				tan = 1 + 0.31755f*angle*angle + 0.20330f*angle*angle*angle*angle;
				tan *= angle;

				return tan;
			}
		}

		///Fast Tan function using taylor series, most accurate
		static FORCEINLINE float FastTan1(float angle) noexcept
		{
			float tan = 0;


			///if the angle is not on the accepted range
			if (angle < 0 || angle > HALFPI)
			{
				return tanf(angle);
			}
			else
			{
				tan = 1 + 0.3333314036f*Pow(angle, 2) + 0.1333923995f*Pow(angle, 4) + 0.0533740603f*Pow(angle, 6)
					+ 0.0245650893f*Pow(angle, 8) + 0.0029005250f*Pow(angle, 10)
					+ 0.0095168091f*Pow(angle, 12);
				tan *= angle;

				return tan;
			}

		}

		///Fast arcSin function using taylor series, fastest
		static FORCEINLINE float FastASin0(float angle) noexcept
		{
			float asin = 0;

			///if the angle is not on the accepted range
			if (angle < 0 || angle > 1)
			{
				return asinf(angle);
			}
			else
			{
				asin = PI / 2 - Sqrt(1 - angle)*(1.5707288f - 0.2121144f*angle
					+ 0.0742610f*Pow(angle, 2) - 0.0187293f*Pow(angle, 3));

				return asin;
			}
		}

		///Fast arcSin function using taylor series, most accurate
		static FORCEINLINE float FastASin1(float angle) noexcept
		{
			float asin = 0;



			///if the angle is not on the accepted range
			if (angle < 0 || angle > 1)
			{
				return asinf(angle);
			}
			else
			{
				asin = HALFPI - Sqrt(1 - angle)*(1.5707963050f - 0.2145988016f*angle + 0.0889789874f* Pow(angle, 2)
					- 0.0501743046f*Pow(angle, 3) + 0.0308918810f*Pow(angle, 4) - 0.01708812556f*Pow(angle, 5)
					+ 0.0066700901f*Pow(angle, 6) - 0.0012624911f*Pow(angle, 7));

				return asin;
			}

		}

		///Fast aCos function using taylor series, fastest
		static FORCEINLINE float FastACos0(float angle) noexcept
		{
			float acos = 0;


			///if the angle is not on the accepted range
			if (angle < 0 || angle > 1)
			{
				return acosf(angle);
			}
			else
			{
				acos = PI / 2 - FastASin0(angle);

				return acos;
			}

		}

		///Fast aCos function using taylor series, most accurate
		static FORCEINLINE float FastACos1(float angle) noexcept
		{
			float acos = 0;


			///if the angle is not on the accepted range
			if (angle < 0 || angle > 1)
			{
				return acosf(angle);
			}
			else
			{
				acos = PI / 2 - FastASin1(angle);

				return acos;
			}

		}

		///Fast aTan function using taylor series, fastest 
		static FORCEINLINE float FastATan0(float angle) noexcept
		{
			float atan = 0;



			///if the angle is not on the accepted range
			if (angle < -1 || angle > 1)
			{
				return atanf(angle);
			}
			else
			{
				atan = 0.9998660f*angle - 0.3302995f*Pow(angle, 3) + 0.1801410f* Pow(angle, 5) - 0.0851330f*Pow(angle, 7)
					+ 0.0208351f*Pow(angle, 9);

				return atan;
			}

		}

		///Fast aTan function using taylor series, most accurate
		static FORCEINLINE float FastATan1(float angle) noexcept
		{ 
			float atan = 0;



			///if the angle is not on the accepted range
			if (angle < -1 || angle > 1)
			{
				return atanf(angle);
			}
			else
			{
				atan = 1 - 0.3333314528f*Pow(angle, 2) + 0.1999355085f*Pow(angle, 4) - 0.1420889944f*Pow(angle, 6)
					+ 0.1065626393f*Pow(angle, 8) - 0.0752896400f*Pow(angle, 10) + 0.0429096138f*Pow(angle, 12)
					- 0.0161657367f*Pow(angle, 14) + 0.0028662257f*Pow(angle, 16);
				atan *= angle;

				return atan;
			}

		}

		///Trig Atan2
		static FORCEINLINE float aTan2(float Y, float X) noexcept { return atan2f(Y, X); };

		///Transforms the Degree to its 0-360° range equivalent
		static float UnwindDegree(float f) noexcept;

		///Transforms the Radian to its 0-2PI range equivalent
		static float UnwindRadian(float f) noexcept;

		const float small_number = (1.e-8f);
		const float float_small_number = (1.e-6f);
		const float kinda_small_number = (1.e-4f);
		const float big_number = (3.4e+38f);

		const float delta = 0.00001f;
		const float epsilon = std::numeric_limits<float>::epsilon();

		///Lerp the value
		template <class T, class U>
		static FORCEINLINE T Lerp(const T& A, const T& B, const U& Alpha)
		{
			return (T)(A + (B - A)*Alpha);
		}

		///returns the Square of any var type
		template <class T>
		static FORCEINLINE T Square(const T A) noexcept
		{
			return A*A;
		}

		///return the min between two vars
		template <class T>
		static FORCEINLINE T Min(const T A, const T B) noexcept
		{
			return (A <= B) ? A : B;
		}

		///returns the max between two vars
		template < class T>
		static FORCEINLINE T Max(const T A, const T B) noexcept
		{
			return (A >= B) ? A : B;
		}

		///returns the min between 3 vars
		template <class T>
		static FORCEINLINE T Min3(const T A, const T B, const T C)
		{
			return Min(Min(A, B), C);
		}

		///returns the max between 3 vars
		template <class T>
		static FORCEINLINE T Max3(const T A, const T B, const T C)
		{
			return Max(Max(A, B), C);
		}

		///Clamps the val between min and max
		template <class T>
		static FORCEINLINE T Clamp(const T val, const T min, const T max)
		{
			return Min(Max(val, min), max);
		}

		///Clamps the val between 0 and 1
		template <typename T>
		static FORCEINLINE T Clamp01(T val)
		{
			return Max(Min(val, (T)1), (T)0);
		}

		static FORCEINLINE constexpr bool Equals(float n1, float n2, float precision) noexcept
		{
			return (n1 - precision >= n2 && n1 + precision <= n2);
		}
	};
};

