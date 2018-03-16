#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Quaternion
	{
	public:
		Quaternion(eINIT init);
		Quaternion(float x, float  y, float  z, float  w);
		Quaternion(Vector3 V, float w);

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

		union {
			Vector3 m_v;
			struct { float m_x, m_y, m_z; };
		};
		float m_w;
	};
}