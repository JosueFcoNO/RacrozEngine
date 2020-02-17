#pragma once
#include "CbnUtilitiesPCH.h"
#include "RZQuaternion.h"

namespace rczEngine
{

	Quaternion::Quaternion(eInit init) noexcept
	{
		if (init == eInit::None || init == eInit::Zero)
		{
			m_v.m_x = 0;
			m_v.m_z = 0;
			m_v.m_y = 0;
			m_w = 0;
		}
		else
		{
			m_v.m_x = 0;
			m_v.m_z = 0;
			m_v.m_y = 0;
			m_w = 1;
		}
	}

	Quaternion::Quaternion(float x, float  y, float  z, float  w) noexcept
	{
		m_v.m_x = x;
		m_v.m_y = y;
		m_v.m_z = z;
		m_w = w;
	}

	Quaternion::Quaternion(Vector3 V, float w) noexcept
	{
		m_v.m_x = V.m_x;
		m_v.m_y = V.m_y;
		m_v.m_z = V.m_z;


		m_w = w;
	}


	float Quaternion::Magnitude() const noexcept
	{
		return Math::Sqrt((Math::Square(m_v.m_x) + Math::Square(m_v.m_y) + Math::Square(m_v.m_z) + Math::Square(m_w)));
	}

	void Quaternion::Negate() noexcept
	{
		m_v.m_x *= -1;
		m_v.m_y *= -1;
		m_v.m_z *= -1;
		m_w *= -1;
	}

	void Quaternion::Conjugate() noexcept
	{
		m_v.m_x *= -1;
		m_v.m_y *= -1;
		m_v.m_z *= -1;

	}

	Quaternion Quaternion::GetConjugate() const noexcept
	{
		Quaternion Temp = *this;
		Temp.m_v.m_x *= -1;
		Temp.m_v.m_y *= -1;
		Temp.m_v.m_z *= -1;

		return Temp;
	}

	void Quaternion::Inverse() noexcept
	{
		Conjugate();
		const float Mag = Magnitude();
		m_v.m_x /= Mag;
		m_v.m_y /= Mag;
		m_v.m_z /= Mag;
		m_w /= Mag;
	}

	Quaternion Quaternion::GetInverse() const noexcept
	{
		Quaternion Temp = *this;
		Temp.Conjugate();
		const float Mag = Magnitude();
		Temp.m_v.m_x /= Mag;
		Temp.m_v.m_y /= Mag;
		Temp.m_v.m_z /= Mag;
		Temp.m_w /= Mag;

		return Temp;
	}

	void Quaternion::Normalize() noexcept
	{
		const float w2 = Math::Square(m_w);
		const float y2 = Math::Square(m_v.m_y);
		const float x2 = Math::Square(m_v.m_x);
		const float z2 = Math::Square(m_v.m_z);

		const float mag = sqrt(w2 + x2 + y2 + z2);

		m_w = m_w / mag;
		m_v.m_x /= mag;
		m_v.m_y /= mag;
		m_v.m_z /= mag;
	}

	Quaternion Quaternion::GetNormalized() noexcept
	{
		Quaternion q = *this;
		q.Normalize();
		return q;
	}

	void Quaternion::Pow(float exp) noexcept
	{
		if (Math::fAbs(m_w) < .9999f) {
			const float alpha = Math::aCos(m_w);
			const float newAlpha = alpha * exp;
			m_w = Math::Cos(newAlpha);
			const float mult = sin(newAlpha) / sin(alpha);
			m_v *= mult;
		}
	}

	Quaternion Quaternion::GetPow(float exp) noexcept
	{
		Quaternion Q = *this;
		Q.Pow(exp);
		return Q;
	}

	Matrix4 Quaternion::GetAsMatrix4() noexcept
	{
		Matrix4 M(GetAsMatrix3());
		return M;
	}

	Matrix3 Quaternion::GetAsMatrix3() noexcept
	{
		Matrix3 M(eInit::None);
		const float xx = Math::Square(m_v.m_x), yy = Math::Square(m_v.m_y), zz = Math::Square(m_v.m_z);
		const float x = m_v.m_x, y = m_v.m_y, z = m_v.m_z, w = m_w;
		M.m_rows[0] = { 1 - (2 * yy) - (2 * zz),	(2 * x*y) + (2 * z*w),	(2 * x*z) - (2 * w*y)};
		M.m_rows[1] = { (2 * x*y) - (2 * z*w),	1 - (2 * xx) - (2 * zz),	(2 * y*z) + (2 * w*x) };
		M.m_rows[2] = { (2 * x*z) + (2 * w*y),	(2 * y*z) - (2 * w*x),	1 - (2 * xx) - (2 * yy) };

		return M;
	}

	Vector3 Quaternion::RotateVector(const Vector3 & rotateV, float scale) const noexcept
	{
		const auto rotateQ = Quaternion(rotateV, 0);

		const auto thisQ = ((*this) * scale);
		const auto thisQInverse = thisQ.GetInverse();

		const auto result = (thisQInverse * rotateQ) * thisQ;

		return result.m_v;
	}

	float Quaternion::operator|(Quaternion rh) noexcept
	{
		return m_w * rh.m_w + (m_v | rh.m_v);
	}

	Quaternion Quaternion::operator*(const Quaternion & rh) const noexcept
	{
		Quaternion Q(eInit::Zero);
		Q.m_w = (m_w * rh.m_w) - (m_v | rh.m_v);
		Q.m_v = (rh.m_v * m_w) + (m_v * rh.m_w) + (m_v ^ rh.m_v);

		return Q;
	}

	void Quaternion::operator*=(Quaternion rh) noexcept
	{
		Quaternion Q(eInit::Zero);
		Q.m_w = m_w*rh.m_w - (m_v | rh.m_v);
		Q.m_v = rh.m_v*m_w + m_v*rh.m_w + m_v^rh.m_v;

		*this = Q;
	}

	Quaternion Quaternion::operator*(float rh) const noexcept
	{
		Quaternion Q(eInit::None);
		Q = *this;
		Q.m_w *= rh;
		Q.m_v *= rh;
		return Q;
	}

	void Quaternion::operator*=(float rh) noexcept
	{
		m_w *= rh;
		m_v *= rh;
	}

	Quaternion Quaternion::GetDifference(Quaternion lh, Quaternion rh) noexcept
	{
		Quaternion Q(eInit::None);
		Q = lh.GetInverse();
		Q = Q*rh;

		return Q;
	}

	Quaternion Quaternion::Slerp(Quaternion q1, Quaternion q2, float t) noexcept
	{
		Quaternion Q(eInit::None);

		float cosOmega = q1.m_w*q2.m_w + Math::Square(q1.m_v.m_x)+ Math::Square(q1.m_v.m_y)+ Math::Square(q1.m_v.m_z);

		if (cosOmega < 0.0f) {
			q2.m_w = -q2.m_w;
			q2.m_v.m_x = -q2.m_v.m_x;
			q2.m_v.m_y = -q2.m_v.m_y;
			q2.m_v.m_z = -q2.m_v.m_z;
			cosOmega = -cosOmega;
		}

		auto k0 = 0.0f;
		auto k1 = 0.0f;

		if (cosOmega > 0.9999f) {
			k0 = 1.0f-t;
			k1 = t;
		}
		else {

			const float sinOmega = sqrt(1.0f - cosOmega*cosOmega);
			const float omega = atan2(sinOmega, cosOmega);
			const float oneOverSinOmega = 1.0f / sinOmega;
			k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
			k1 = sin(t * omega) * oneOverSinOmega;
		}
		Q.m_w = q1.m_w*k0 + q2.m_w*k1;
		Q.m_v.m_x = q1.m_v.m_x*k0 + q2.m_v.m_x*k1;
		Q.m_v.m_y = q1.m_v.m_y*k0 + q2.m_v.m_y*k1;
		Q.m_v.m_z = q1.m_v.m_z*k0 + q2.m_v.m_z*k1;
		return Q;
	}

}