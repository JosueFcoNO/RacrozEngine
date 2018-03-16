#pragma once
#include <RZUtilityPCH.h>
#include "RZVector4D.h"

namespace rczEngine
{

	///////////////////////////////////////////////
	////////VECTOR 4D
	///////////////////////////////////////////////

	const Vector4 Vector4::ZERO(INIT_ZERO);

	Vector4::Vector4(eINIT init)
	{
		if (init == INIT_NONE)
		{
			return;
		} else
		if (init == INIT_UNIT)
		{
			*this = ZERO;
			m_x = 1;
		}
		else
		{
			*this = ZERO;
		}
	}

	Vector4::Vector4(Vector3 v3)
	{
		m_x = v3.m_x;
		m_y = v3.m_y;
		m_z = v3.m_z;
		m_w = 0;
	}

	Vector4::Vector4(Vector2 v2)
	{
		m_x = v2.m_x;
		m_y = v2.m_y;
		m_z = 0;
		m_w = 0;
	}

	Vector4::Vector4(Vector2I v2i)
	{
		m_x = static_cast<float>(v2i.m_x);
		m_y = static_cast<float>(v2i.m_y);
		m_z = 0;
		m_w = 0;
	}

	Vector4::Vector4(const float& x, const float& y, const float& z, const float& w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	void Vector4::Set(const float& x, const float& y, const float& z, const float& w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	void Vector4::Scale(const float& Scale)
	{
		m_x *= Scale;
		m_y *= Scale;
		m_z *= Scale;
		m_w *= Scale;
	}

	Vector4 Vector4::operator+(const Vector4& v) const
	{
		return Vector4(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z, m_w + v.m_w);
	}

	Vector4 Vector4::operator-(const Vector4& v) const
	{
		return Vector4(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z, m_w - v.m_w);
	}

	Vector4 Vector4::operator*(const Vector4& v) const
	{
		return Vector4(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z, m_w * v.m_w);
	}

	Vector4 Vector4::operator*(float f)
	{
		return Vector4(m_x * f, m_y * f, m_z * f, m_w *f);
	}

	Vector4 Vector4::operator/(const Vector4& v) const
	{
		return Vector4(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z, m_w / v.m_w);
	}

	Vector4 Vector4::operator/(float f)
	{
		return Vector4(m_x / f, m_y / f, m_z / f, m_w / f);
	}

	Vector4& Vector4::operator+=(const Vector4& v)
	{
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;
		m_w += v.m_w;

		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& v)
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
		m_z -= v.m_z;
		m_w -= v.m_w;

		return *this;
	}



	Vector4& Vector4::operator*=(const Vector4& v)
	{
		m_x *= v.m_x;
		m_y *= v.m_y;
		m_z *= v.m_z;
		m_w *= v.m_w;

		return *this;
	}

	Vector4& Vector4::operator*=(float f)
	{
		m_x *= f;
		m_y *= f;
		m_z *= f;
		m_w *= f;

		return *this;
	}

	Vector4& Vector4::operator/=(const Vector4& v)
	{
		m_x /= v.m_x;
		m_y /= v.m_y;
		m_z /= v.m_z;
		m_w /= v.m_w;

		return *this;
	}

	Vector4& Vector4::operator/=(float f)
	{
		m_x /= f;
		m_y /= f;
		m_z /= f;
		m_w /= f;

		return *this;
	}

	bool Vector4::operator==(const Vector4& v) const
	{
		return (m_x == v.m_x && m_y == v.m_y && m_z == v.m_z && m_w == v.m_w);
	}

	bool Vector4::operator!=(const Vector4& v) const
	{
		return !(m_x == v.m_x && m_y == v.m_y && m_z == v.m_z && m_w != v.m_w);
	}

	bool Vector4::operator<(const Vector4& v) const
	{
		return (m_x < v.m_x && m_y < v.m_y && m_z < v.m_z && m_w < v.m_w);
	}

	bool Vector4::operator>(const Vector4& v) const
	{
		return (m_x > v.m_x && m_y > v.m_y && m_z > v.m_z && m_w > v.m_w);
	}

	bool Vector4::operator<=(const Vector4& v) const
	{
		return (m_x <= v.m_x && m_y <= v.m_y && m_z <= v.m_z && m_w <= v.m_w);
	}

	bool Vector4::operator>=(const Vector4& v) const
	{
		return (m_x >= v.m_x && m_y >= v.m_y && m_z >= v.m_z && m_w >= v.m_w);
	}

	float Vector4::operator|(const Vector4& v) const
	{
		float Dot = 0;

		Dot = m_x * v.m_x + m_y * v.m_y + m_z * v.m_z + m_w * v.m_w;

		return Dot;
	}

	float Vector4::operator^(const Vector4& v) const
	{
		return m_x*v.m_y - m_y*v.m_x;
	}

	void Vector4::Normalize()
	{
		float Mag = Magnitude();
		m_x /= Mag;
		m_y /= Mag;
		m_z /= Mag;
		m_w /= Mag;
	}

	Vector4 Vector4::GetNormalized()
	{
		Vector4 Temp(INIT_NONE);
		Temp = *this;

		Temp.Normalize();
		return Temp;
	}


	float Vector4::Magnitude()
	{
		return Math::Sqrt((Math::Square(m_x) + Math::Square(m_y) + Math::Square(m_z) + Math::Square(m_w)));
	}
}