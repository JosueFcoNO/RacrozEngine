#pragma once

namespace rczEngine
{
	///Class to use Radians, implements some functionality and avoids confusion
	class RZ_EXP Radian
	{
	public:
		Radian(float F) noexcept;

		///return the value as a float
		float Value() const noexcept { return m_Value; };
		///returns a Degree object
		Degree ValueDegree() const noexcept;

		///unwinds the radians, calls the real function from Math
		void UnwindRadian(float f);

		Radian operator= (float f) noexcept { return Radian(f); };
		Radian operator= (int32 i) noexcept { return Radian(gsl::narrow_cast<float>(i)); };

		float operator+(Radian R) noexcept { return m_Value + R.m_Value; };
		float operator-(Radian R) noexcept { return m_Value - R.m_Value; };
		float operator*(Radian R) noexcept { return m_Value * R.m_Value; };
		float operator/(Radian R) noexcept { return m_Value / R.m_Value; };

		float operator+(float f) noexcept { return m_Value + f; };
		float operator-(float f) noexcept { return m_Value - f; };
		float operator*(float f) noexcept { return m_Value * f; };
		float operator/(float f) noexcept { return m_Value / f; };

		friend float operator+(float lhs, const Radian& rhs) noexcept { return lhs + rhs.m_Value; };
		friend float operator-(float lhs, const Radian& rhs) noexcept { return lhs + rhs.m_Value; };
		friend float operator*(float lhs, const Radian& rhs) noexcept { return lhs + rhs.m_Value; };
		friend float operator/(float lhs, const Radian& rhs) noexcept { return lhs + rhs.m_Value; };

		Radian& operator +=(Radian R) noexcept { (*this).m_Value += R.m_Value; return *this; };
		Radian& operator -=(Radian R) noexcept { (*this).m_Value -= R.m_Value; return *this; };
		Radian& operator *=(Radian R) noexcept { (*this).m_Value *= R.m_Value; return *this; };
		Radian& operator /=(Radian R) noexcept { (*this).m_Value /= R.m_Value; return *this; };

		Radian& operator +=(float f) noexcept { (*this).m_Value += f; return *this; };
		Radian& operator -=(float f) noexcept { (*this).m_Value -= f; return *this; };
		Radian& operator *=(float f) noexcept { (*this).m_Value *= f; return *this; };
		Radian& operator /=(float f) noexcept { (*this).m_Value /= f; return *this; };

		friend float operator+=(float& lhs, const Radian& rhs) noexcept { return lhs += rhs.m_Value; };
		friend float operator*=(float& lhs, const Radian& rhs) noexcept { return lhs *= rhs.m_Value; };
		friend float operator-=(float& lhs, const Radian& rhs) noexcept { return lhs -= rhs.m_Value; };
		friend float operator/=(float& lhs, const Radian& rhs) noexcept { return lhs /= rhs.m_Value; };

		bool operator<(const float& v)  const noexcept { return m_Value < v; }
		bool operator>(const float& v)  const noexcept { return m_Value > v; };
		bool operator==(const float& v) const noexcept { return m_Value == v; };
		bool operator!=(const float& v) const noexcept { return m_Value != v; };
		bool operator<=(const float& v) const noexcept { return m_Value <= v; };
		bool operator>=(const float& v) const noexcept { return m_Value >= v; };

		bool operator<(const Radian& v)  const noexcept { return m_Value < v.m_Value; }
		bool operator>(const Radian& v)  const noexcept { return m_Value > v.m_Value; };
		bool operator==(const Radian& v) const noexcept { return m_Value == v.m_Value; };
		bool operator!=(const Radian& v) const noexcept { return m_Value != v.m_Value; };
		bool operator<=(const Radian& v) const noexcept { return m_Value <= v.m_Value; };
		bool operator>=(const Radian& v) const noexcept { return m_Value >= v.m_Value; };

		friend bool operator<(const float& lhs, const Radian& rhs)  noexcept { return rhs.m_Value < lhs; }
		friend bool operator>(const float& lhs, const Radian& rhs)  noexcept { return rhs.m_Value > lhs; };
		friend bool operator==(const float& lhs, const Radian& rhs) noexcept { return rhs.m_Value == lhs; };
		friend bool operator!=(const float& lhs, const Radian& rhs) noexcept { return rhs.m_Value != lhs; };
		friend bool operator<=(const float& lhs, const Radian& rhs) noexcept { return rhs.m_Value <= lhs; };
		friend bool operator>=(const float& lhs, const Radian& rhs) noexcept { return rhs.m_Value >= lhs; };

		float m_Value;

	};


}