#pragma once

namespace rczEngine
{
	class RZ_EXP Quaternion
	{
	public:
		Quaternion(eInit init) noexcept;
		Quaternion(float x, float  y, float  z, float  w) noexcept;
		Quaternion(Vector3 V, float w) noexcept;

		float Magnitude() const noexcept;
		void Negate() noexcept;
		void Conjugate() noexcept;
		Quaternion GetConjugate() const noexcept;
		void Inverse() noexcept;
		Quaternion GetInverse() const noexcept;
		void Pow(float exp) noexcept;
		Quaternion GetPow(float exp) noexcept;
		
		void Normalize() noexcept;
		Quaternion GetNormalized() noexcept;

		Matrix4 GetAsMatrix4() noexcept;
		Matrix3 GetAsMatrix3() noexcept;


		Vector3 RotateVector(const Vector3 & rotateV, float scale) const noexcept;

		float operator|(Quaternion rh) noexcept;
		Quaternion operator*(const Quaternion& rh) const noexcept;
		void operator*=(Quaternion rh) noexcept;

		Quaternion operator*(float rh) const noexcept;
		void operator*=(float rh) noexcept;

		static Quaternion GetDifference(Quaternion lh, Quaternion rh) noexcept;
		static Quaternion Slerp(Quaternion q1, Quaternion q2, float t) noexcept;
		static Quaternion FromAxisAngle(const Vector3& axis, float angle, bool radians = false) noexcept;

		Vector3 m_v;
		float m_w;
	};
}