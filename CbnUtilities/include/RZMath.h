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

		static Vector3 PosToPolar3D(Vector3 pos) noexcept;
		static Vector2 PolarToPos2D(Vector3 polar) noexcept;
		static Vector3 PosToPolar2D(Vector2 pos) noexcept;
		static Vector3 PolarToPos3D(Vector3 polar) noexcept;
		
		static Vector3 cubify(const Vector3& s) noexcept;
		static Vector3 SphereToCubePoint(const Vector3& spherePoint) noexcept;
		static Vector3 CubeToSphere(const Vector3 &cubepoint) noexcept;

		///Elevate a float to ipow
		template<class valueType, class powerType>
		static FORCEINLINE valueType Pow(valueType V, powerType ipow) noexcept { return std::pow(V, ipow); };
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

		///Trig Atan2
		static FORCEINLINE float aTan2(float Y, float X) noexcept { return atan2f(Y, X); };

		static FORCEINLINE constexpr float Sign(float x) noexcept	{ return (x >= 0.0f) ? 1.0f : -1.0f; };

		static FORCEINLINE constexpr int Sign(int x) noexcept { return (x >= 0) ? 1 : -1;	};

		static Vector2 Sign(Vector2 x) noexcept;

		static Vector2I Sign(Vector2I x) noexcept;

		static Vector3 Sign(Vector3 x) noexcept;

		static Vector4 Sign(Vector4 x) noexcept;

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
		static FORCEINLINE T Lerp(const T& A, const T& B, const U& Alpha) noexcept
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
			return (n1>= n2 - precision && n1 <= n2 + precision);
		}
	};

	class RZ_EXP TrigFast
	{
	public:

		///Fast Sin function using taylor series, fastest
		static float FastSin0(float angle) noexcept;

		///Fast Sin function using taylor series, most accurate
		static float FastSin1(float angle) noexcept;

		///Fast Cos function using taylor series, fastest
		static float FastCos0(float angle) noexcept;

		///Fast Sin function using taylor series, most accurate
		static float FastCos1(float angle) noexcept;

		///Fast Tan function using taylor series, fastest
		static float FastTan0(float angle) noexcept;

		///Fast Tan function using taylor series, most accurate
		static float FastTan1(float angle) noexcept;

		///Fast arcSin function using taylor series, fastest
		static float FastASin0(float angle) noexcept;

		///Fast arcSin function using taylor series, most accurate
		static float FastASin1(float angle) noexcept;

		///Fast aCos function using taylor series, fastest
		static float FastACos0(float angle) noexcept;

		///Fast aCos function using taylor series, most accurate
		static float FastACos1(float angle) noexcept;

		///Fast aTan function using taylor series, fastest 
		static float FastATan0(float angle) noexcept;

		///Fast aTan function using taylor series, most accurate
		static float FastATan1(float angle) noexcept;
	};
};

