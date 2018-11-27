#pragma once

namespace rczEngine{

	///Class to use Degrees and make sure there's no confusion on what a function returns, also adds some functionality
	class RZ_EXP Degree
	{
	public:
		Degree() { m_Value = 0.0f; };
		Degree(float F);

		///Returns the value as float 
		float Value() { return m_Value; };
		///Returns the value as a Radian object that has the Value in Radians inside
		Radian ValueRadian();
		///Returns a copy of this
		Degree ValueDegree() {
			return *this;
		};

		///Unwinds the degrees, calls the real function from Math
		void UnwindDegree(float F);

		///Equals a Radian to a Degree, conversion included
		Degree operator= (Radian R) { Degree Temp(R.ValueDegree().m_Value); return Temp; };
		///Equals a float to a Degree
		Degree operator= (float f) { Degree Temp(f); return Temp; };
		///Equals a Degree to another
		Degree operator= (Degree D) { return D; };
		///Equals an int to a degree
		Degree operator= (int32 i) { Degree Temp((float)i); return Temp; };

		float operator+(Degree R) { return m_Value + R.m_Value; };
		float operator-(Degree R) { return m_Value - R.m_Value; };
		float operator*(Degree R) { return m_Value * R.m_Value; };
		float operator/(Degree R) { return m_Value / R.m_Value; };

		float operator+(float f) { return m_Value + f; };
		float operator-(float f) { return m_Value - f; };
		float operator*(float f) { return m_Value * f; };
		float operator/(float f) { return m_Value / f; };

		friend float operator+(float lhs, const Degree& rhs) { return lhs + rhs.m_Value; };
		friend float operator-(float lhs, const Degree& rhs) { return lhs + rhs.m_Value; };
		friend float operator*(float lhs, const Degree& rhs) { return lhs + rhs.m_Value; };
		friend float operator/(float lhs, const Degree& rhs) { return lhs + rhs.m_Value; };

		Degree& operator +=(Degree R) { (*this).m_Value += R.m_Value; return *this; };
		Degree& operator -=(Degree R) { (*this).m_Value -= R.m_Value; return *this; };
		Degree& operator *=(Degree R) { (*this).m_Value *= R.m_Value; return *this; };
		Degree& operator /=(Degree R) { (*this).m_Value /= R.m_Value; return *this; };

		Degree& operator +=(float f) { (*this).m_Value += f; return *this; };
		Degree& operator -=(float f) { (*this).m_Value -= f; return *this; };
		Degree& operator *=(float f) { (*this).m_Value *= f; return *this; };
		Degree& operator /=(float f) { (*this).m_Value /= f; return *this; };

		friend float operator+=(float& lhs, const Degree& rhs) { return lhs += rhs.m_Value; };
		friend float operator*=(float& lhs, const Degree& rhs) { return lhs *= rhs.m_Value; };
		friend float operator-=(float& lhs, const Degree& rhs) { return lhs -= rhs.m_Value; };
		friend float operator/=(float& lhs, const Degree& rhs) { return lhs /= rhs.m_Value; };

		bool operator<(const float& v) { return m_Value < v; }
		bool operator>(const float& v) { return m_Value > v; };
		bool operator==(const float& v) { return m_Value == v; };
		bool operator!=(const float& v) { return m_Value != v; };
		bool operator<=(const float& v) { return m_Value <= v; };
		bool operator>=(const float& v) { return m_Value >= v; };

		bool operator<(const Degree& v) { return m_Value < v.m_Value; }
		bool operator>(const Degree& v) { return m_Value > v.m_Value; };
		bool operator==(const Degree& v) { return m_Value == v.m_Value; };
		bool operator!=(const Degree& v) { return m_Value != v.m_Value; };
		bool operator<=(const Degree& v) { return m_Value <= v.m_Value; };
		bool operator>=(const Degree& v) { return m_Value >= v.m_Value; };

		friend bool operator<(const float& lhs, const Degree& rhs) { return rhs.m_Value < lhs; }
		friend bool operator>(const float& lhs, const Degree& rhs) { return rhs.m_Value > lhs; };
		friend bool operator==(const float& lhs, const Degree& rhs) { return rhs.m_Value == lhs; };
		friend bool operator!=(const float& lhs, const Degree& rhs) { return rhs.m_Value != lhs; };
		friend bool operator<=(const float& lhs, const Degree& rhs) { return rhs.m_Value <= lhs; };
		friend bool operator>=(const float& lhs, const Degree& rhs) { return rhs.m_Value >= lhs; };

		float m_Value;

	};
}