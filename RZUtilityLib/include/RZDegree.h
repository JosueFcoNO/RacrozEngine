#pragma once

namespace rczEngine
{
	///Class to use Degrees and make sure there's no confusion on what a function returns, also adds some functionality
	class RZ_EXP Degree
	{
	public:
		Degree() noexcept : m_Value(0.0f) {};
		Degree(float value) noexcept : m_Value(value) {};

		///Returns the value as float 
		FORCEINLINE float Value() const noexcept { return m_Value; };

		///Returns the value as a Radian object that has the Value in Radians inside
		FORCEINLINE Radian ValueRadian() const;

		///Unwinds the degrees, calls the real function from Math
		FORCEINLINE void Degree::UnwindDegree(float F);

		///Equals a Radian to a Degree, conversion included
		Degree operator= (Radian R) noexcept { return R.ValueDegree(); };
		///Equals a float to a Degree
		Degree operator= (float f) noexcept { return Degree{f}; };
		///Equals an int to a degree
		Degree operator= (int32 i) noexcept { return gsl::narrow_cast<float>(i); };

		float operator+(Degree R) noexcept { return m_Value + R.m_Value; };
		float operator-(Degree R) noexcept { return m_Value - R.m_Value; };
		float operator*(Degree R) noexcept { return m_Value * R.m_Value; };
		float operator/(Degree R) noexcept { return m_Value / R.m_Value; };

		float operator+(float f) noexcept { return m_Value + f; };
		float operator-(float f) noexcept { return m_Value - f; };
		float operator*(float f) noexcept { return m_Value * f; };
		float operator/(float f) noexcept { return m_Value / f; };

		friend float operator+(float lhs, const Degree& rhs) noexcept { return lhs + rhs.m_Value; };
		friend float operator-(float lhs, const Degree& rhs) noexcept { return lhs + rhs.m_Value; };
		friend float operator*(float lhs, const Degree& rhs) noexcept { return lhs + rhs.m_Value; };
		friend float operator/(float lhs, const Degree& rhs) noexcept { return lhs + rhs.m_Value; };

		Degree& operator +=(Degree R) noexcept { (*this).m_Value += R.m_Value; return *this; };
		Degree& operator -=(Degree R) noexcept { (*this).m_Value -= R.m_Value; return *this; };
		Degree& operator *=(Degree R) noexcept { (*this).m_Value *= R.m_Value; return *this; };
		Degree& operator /=(Degree R) noexcept { (*this).m_Value /= R.m_Value; return *this; };

		Degree& operator +=(float f) noexcept { (*this).m_Value += f; return *this; };
		Degree& operator -=(float f) noexcept { (*this).m_Value -= f; return *this; };
		Degree& operator *=(float f) noexcept { (*this).m_Value *= f; return *this; };
		Degree& operator /=(float f) noexcept { (*this).m_Value /= f; return *this; };

		friend float operator+=(float& lhs, const Degree& rhs) noexcept { return lhs += rhs.m_Value; };
		friend float operator*=(float& lhs, const Degree& rhs) noexcept { return lhs *= rhs.m_Value; };
		friend float operator-=(float& lhs, const Degree& rhs) noexcept { return lhs -= rhs.m_Value; };
		friend float operator/=(float& lhs, const Degree& rhs) noexcept { return lhs /= rhs.m_Value; };

		bool operator<(const float& v)  noexcept { return m_Value < v; }
		bool operator>(const float& v)  noexcept { return m_Value > v; };
		bool operator==(const float& v) noexcept { return m_Value == v; };
		bool operator!=(const float& v) noexcept { return m_Value != v; };
		bool operator<=(const float& v) noexcept { return m_Value <= v; };
		bool operator>=(const float& v) noexcept { return m_Value >= v; };

		bool operator<(const Degree& v)  noexcept { return m_Value < v.m_Value; }
		bool operator>(const Degree& v)  noexcept { return m_Value > v.m_Value; };
		bool operator==(const Degree& v) noexcept { return m_Value == v.m_Value; };
		bool operator!=(const Degree& v) noexcept { return m_Value != v.m_Value; };
		bool operator<=(const Degree& v) noexcept { return m_Value <= v.m_Value; };
		bool operator>=(const Degree& v) noexcept { return m_Value >= v.m_Value; };

		friend bool operator<(const float& lhs, const Degree& rhs)  noexcept { return rhs.m_Value < lhs; }
		friend bool operator>(const float& lhs, const Degree& rhs)  noexcept { return rhs.m_Value > lhs; };
		friend bool operator==(const float& lhs, const Degree& rhs) noexcept { return rhs.m_Value == lhs; };
		friend bool operator!=(const float& lhs, const Degree& rhs) noexcept { return rhs.m_Value != lhs; };
		friend bool operator<=(const float& lhs, const Degree& rhs) noexcept { return rhs.m_Value <= lhs; };
		friend bool operator>=(const float& lhs, const Degree& rhs) noexcept { return rhs.m_Value >= lhs; };

		float m_Value;

	};
}