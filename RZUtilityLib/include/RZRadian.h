#pragma once

namespace rczEngine
{
	///Class to use Radians, implements some functionality and avoids confusion
	class RZ_EXP Radian
	{
	public:
		Radian() { m_Value = 0.0f; };
		Radian(float F);

		///return the value as a float
		float Value() { return m_Value; };
		///returns a copy of this
		Radian ValueRadian() { Radian Temp(*this); return Temp; };
		///returns a Degree object
		Degree ValueDegree();
		
		///unwinds the radians, calls the real function from Math
		void UnwindRadian(float f);


		Radian operator= (Radian R) { return Radian(R.m_Value); };
		Radian operator= (float f) { return Radian(f); };
		Radian operator= (int32 i) { return Radian((float)i); };

		float operator+(Radian R) { return m_Value + R.m_Value; };
		float operator-(Radian R) { return m_Value - R.m_Value; };
		float operator*(Radian R) { return m_Value * R.m_Value; };
		float operator/(Radian R) { return m_Value / R.m_Value; };

		float operator+(float f) { return m_Value + f; };
		float operator-(float f) { return m_Value - f; };
		float operator*(float f) { return m_Value * f; };
		float operator/(float f) { return m_Value / f; };

		friend float operator+(float lhs, const Radian& rhs) { return lhs + rhs.m_Value; };
		friend float operator-(float lhs, const Radian& rhs) { return lhs + rhs.m_Value; };
		friend float operator*(float lhs, const Radian& rhs) { return lhs + rhs.m_Value; };
		friend float operator/(float lhs, const Radian& rhs) { return lhs + rhs.m_Value; };

		Radian& operator +=(Radian R) { (*this).m_Value += R.m_Value; return *this; };
		Radian& operator -=(Radian R) { (*this).m_Value -= R.m_Value; return *this; };
		Radian& operator *=(Radian R) { (*this).m_Value *= R.m_Value; return *this; };
		Radian& operator /=(Radian R) { (*this).m_Value /= R.m_Value; return *this; };

		Radian& operator +=(float f) { (*this).m_Value += f; return *this; };
		Radian& operator -=(float f) { (*this).m_Value -= f; return *this; };
		Radian& operator *=(float f) { (*this).m_Value *= f; return *this; };
		Radian& operator /=(float f) { (*this).m_Value /= f; return *this; };

		friend float operator+=(float& lhs, const Radian& rhs) { return lhs += rhs.m_Value; };
		friend float operator*=(float& lhs, const Radian& rhs) { return lhs *= rhs.m_Value; };
		friend float operator-=(float& lhs, const Radian& rhs) { return lhs -= rhs.m_Value; };
		friend float operator/=(float& lhs, const Radian& rhs) { return lhs /= rhs.m_Value; };

		bool operator<(const float& v) { return m_Value < v; }
		bool operator>(const float& v) { return m_Value > v; };
		bool operator==(const float& v) { return m_Value == v; };
		bool operator!=(const float& v) { return m_Value != v; };
		bool operator<=(const float& v) { return m_Value <= v; };
		bool operator>=(const float& v) { return m_Value >= v; };

		bool operator<(const Radian& v) { return m_Value < v.m_Value; }
		bool operator>(const Radian& v) { return m_Value > v.m_Value; };
		bool operator==(const Radian& v) { return m_Value == v.m_Value; };
		bool operator!=(const Radian& v) { return m_Value != v.m_Value; };
		bool operator<=(const Radian& v) { return m_Value <= v.m_Value; };
		bool operator>=(const Radian& v) { return m_Value >= v.m_Value; };

		friend bool operator<(const float& lhs, const Radian& rhs) { return rhs.m_Value < lhs; }
		friend bool operator>(const float& lhs, const Radian& rhs) { return rhs.m_Value > lhs; };
		friend bool operator==(const float& lhs, const Radian& rhs) { return rhs.m_Value == lhs; };
		friend bool operator!=(const float& lhs, const Radian& rhs) { return rhs.m_Value != lhs; };
		friend bool operator<=(const float& lhs, const Radian& rhs) { return rhs.m_Value <= lhs; };
		friend bool operator>=(const float& lhs, const Radian& rhs) { return rhs.m_Value >= lhs; };

		float m_Value;

	};


}