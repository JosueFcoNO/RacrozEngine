#pragma once

namespace rczEngine
{
	class RZ_EXP Quaternion
	{
	public:
		Quaternion(eINIT init);
		Quaternion(float x, float  y, float  z, float  w);
		Quaternion(Vector3 V, float w);

		Quaternion(const Quaternion& other)
		{
			m_v = other.m_v;
			m_w = other.m_w;
		};

		Quaternion& operator=(const Quaternion& other)
		{
			m_v = other.m_v;
			m_w = other.m_w;

			return *this;
		};

		float Magnitude();
		void Negate();
		void Conjugate();
		Quaternion GetConjugate();
		void Inverse();
		Quaternion GetInverse();
		void Pow(float exp);
		Quaternion GetPow(float exp);
		
		void Normalize();
		Quaternion GetNormalized();

		Matrix4 GetAsMatrix4();
		Matrix3 GetAsMatrix3();

		float operator|(Quaternion rh);
		Quaternion operator*(Quaternion rh);
		void operator*=(Quaternion rh);

		Quaternion operator*(float rh);
		void operator*=(float rh);

		static Quaternion GetDifference(Quaternion lh, Quaternion rh);
		static Quaternion Slerp(Quaternion q1, Quaternion q2, float t);

		Vector3 m_v;
		float m_w;
	};
}