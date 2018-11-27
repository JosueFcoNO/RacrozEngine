#pragma once
#include <RZUtilityPCH.h>
#include "RZVector3D.h"

namespace rczEngine
{
	const Vector3 Vector3::ZERO(0,0,0);

	Vector3::Vector3(eINIT init)
	{
		if (init == INIT_NONE)
		{
			return;
		} else
		if (init == INIT_UNIT)
		{
			m_x = 1;
			m_y = 0;
			m_z = 0;
		}
		else
		{
			*this = ZERO;
		}
	}

	Vector3::Vector3(const Vector2& v2)
	{
		m_x = v2.m_x;
		m_y = v2.m_y;
		m_z = 0;
	}

	Vector3::Vector3(const Vector2I& v2i)
	{
		m_x = static_cast<float>(v2i.m_x);
		m_y = static_cast<float>(v2i.m_y);
		m_z = 0;
	}

	Vector3::Vector3(const Vector4& v4)
	{
		m_x = v4.m_x;
		m_y = v4.m_y;
		m_z = v4.m_z;
	}

	Vector3::Vector3(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	///Sets the vector
	void Vector3::Set(const float& x, const float& y, const float& z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void Vector3::Set(const Vector3 & v)
	{
		*this = v;
	}

	///Scales the vector by scale
	void Vector3::Scale(const float& Scale)
	{
		m_x *= Scale;
		m_y *= Scale;
		m_z *= Scale;
	}

	///Returns the distance of the vector3: vhead-vtail.
	float Vector3::Distance(const Vector3 & vhead, const Vector3 & vtail)
	{
		Vector3 temp = vhead - vtail;

		return temp.Magnitude();
	}

	///returns the angle of the x and y components of the vector
	float Vector3::Anglexz(const Vector3 & v, bool getAsDegree)
	{
		Vector3 temp = v;
		temp.Normalize();

		if (getAsDegree)
		{
		return Math::RadiansToDegrees(Math::aTan2(temp.m_z, temp.m_x)+180.0f);
		}

		return Math::aTan2(temp.m_z,temp.m_x)+PI2;
	}

	Vector3 Vector3::operator+(const Vector3& v) const
	{
		return Vector3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
	}

	Vector3 Vector3::operator-(const Vector3& v) const
	{
		return Vector3(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
	}

	Vector3 Vector3::operator*(const Vector3& v) const
	{
		return Vector3(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z);
	}

	Vector3 Vector3::operator*(const float& f) const
	{
		return Vector3(m_x * f, m_y * f, m_z * f);
	}

	Vector3 Vector3::operator/(const Vector3& v) const
	{
		return Vector3(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z);
	}

	Vector3 Vector3::operator/(const float& f) const 
	{
		return Vector3(m_x / f, m_y / f, m_z / f);
	}

	Vector3& Vector3::operator+=(const Vector3& v)
	{
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;

		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& v)
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
		m_z -= v.m_z;

		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& v)
	{
		m_x *= v.m_x;
		m_y *= v.m_y;
		m_z *= v.m_z;

		return *this;
	}

	Vector3& Vector3::operator*=(const float& f)
	{
		m_x *= f;
		m_y *= f;
		m_z *= f;

		return *this;
	}

	Vector3& Vector3::operator/=(const Vector3& v)
	{
		m_x /= v.m_x;
		m_y /= v.m_y;
		m_z /= v.m_z;

		return *this;
	}

	Vector3& Vector3::operator/=(const float& f)
	{
		m_x /= f;
		m_y /= f;
		m_z /= f;

		return *this;
	}

	bool Vector3::operator==(const Vector3& v) const
	{
		return (m_x == v.m_x && m_y == v.m_y && m_z == v.m_z);
	}

	bool Vector3::operator!=(const Vector3& v) const
	{
		return !(m_x == v.m_x && m_y == v.m_y && m_z == v.m_z);
	}

	bool Vector3::operator<(const Vector3& v) const
	{
		return (m_x < v.m_x && m_y < v.m_y && m_z < v.m_z);
	}

	bool Vector3::operator>(const Vector3& v) const
	{
		return (m_x > v.m_x && m_y > v.m_y && m_z > v.m_z);
	}

	bool Vector3::operator<=(const Vector3& v) const
	{
		return (m_x <= v.m_x && m_y <= v.m_y && m_z <= v.m_z);
	}

	bool Vector3::operator>=(const Vector3& v) const
	{
		return (m_x >= v.m_x && m_y >= v.m_y && m_z >= v.m_z);
	}

	float Vector3::operator|(const Vector3& v) const
	{
		float Dot = 0;

		Dot = m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;

		return Dot;
	}

	Vector3 Vector3::operator^(const Vector3& v) const
	{
		float x = m_y*v.m_z - m_z*v.m_y;
		float y = m_z*v.m_x - m_x*v.m_z;
		float z = m_x*v.m_y - m_y*v.m_x;

		return Vector3(x, y, z);
	}

	void Vector3::Normalize()
	{
		float Mag = Magnitude();
		m_x /= Mag;
		m_y /= Mag;
		m_z /= Mag;
	}

	Vector3 Vector3::GetNormalized()
	{
		Vector3 Temp(INIT_NONE);
		Temp = *this;

		Temp.Normalize();
		return Temp;
	}

	float Vector3::Magnitude()
	{
		return Math::Sqrt((Math::Square(m_x) + Math::Square(m_y) + Math::Square(m_z)));
	}

}