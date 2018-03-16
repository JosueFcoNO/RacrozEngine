#pragma once
#include <RZUtilityPCH.h>
#include "RZVector2D.h"

namespace rczEngine {
	Vector2::Vector2(eINIT init)
	{
		if (init == INIT_NONE)
		{
			return;
		}
		else if (init == INIT_UNIT)
		{
			m_x = 1;
			m_y = 0;
		}
		else
		{
			m_x = 0;
			m_y = 0;
		}
	}

	Vector2::Vector2(const Vector2I& v2i)
	{
		m_x = static_cast<float>(v2i.m_x);
		m_y = static_cast<float>(v2i.m_y);
	}

	Vector2::Vector2(const Vector3& v3)
	{
		m_x = v3.m_x;
		m_y = v3.m_y;
	}

	Vector2::Vector2(const Vector4& v4)
	{
		m_x = v4.m_x;
		m_y = v4.m_y;
	}

	Vector2::Vector2(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	void Vector2::Set(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	void Vector2::Scale(float Scale)
	{
		m_x *= Scale;
		m_y *= Scale;
	}

	Vector2 Vector2::operator+(const Vector2& v) const
	{
		return Vector2(m_x + v.m_x, m_y + v.m_y);
	}

	Vector2 Vector2::operator-(const Vector2& v) const
	{
		return Vector2(m_x - v.m_x, m_y - v.m_y);
	}

	Vector2 Vector2::operator*(const Vector2& v) const
	{
		return Vector2(m_x * v.m_x, m_y * v.m_y);
	}

	Vector2 Vector2::operator*(const float& f) const
	{
		return Vector2(m_x * f, m_y * f);
	}

	Vector2 Vector2::operator/(const Vector2& v) const
	{
		return Vector2(m_x / v.m_x, m_y / v.m_y);
	}

	Vector2 Vector2::operator/(const float& f) const
	{
		return Vector2(m_x / f, m_y / f);
	}

	Vector2& Vector2::operator+=(Vector2 v)
	{
		m_x += v.m_x;
		m_y += v.m_y;

		return *this;
	}

	Vector2& Vector2::operator-=(Vector2 v)
	{
		m_x -= v.m_x;
		m_y -= v.m_y;

		return *this;
	}



	Vector2& Vector2::operator*=(Vector2 v)
	{
		m_x *= v.m_x;
		m_y *= v.m_y;

		return *this;
	}

	Vector2& Vector2::operator*=(float f)
	{
		m_x *= f;
		m_y *= f;

		return *this;
	}

	Vector2& Vector2::operator/=(Vector2 v)
	{
		m_x /= v.m_x;
		m_y /= v.m_y;

		return *this;
	}

	Vector2& Vector2::operator/=(float f)
	{
		m_x /= f;
		m_y /= f;

		return *this;
	}

	bool Vector2::operator==(Vector2 v)
	{
		return (m_x == v.m_x && m_y == v.m_y);
	}

	bool Vector2::operator!=(Vector2 v)
	{
		return !(m_x == v.m_x && m_y == v.m_y);
	}

	bool Vector2::operator<(Vector2 v)
	{
		return (m_x < v.m_x && m_y < v.m_y);
	}

	bool Vector2::operator>(Vector2 v)
	{
		return (m_x > v.m_x && m_y > v.m_y);
	}

	bool Vector2::operator<=(Vector2 v)
	{
		return (m_x <= v.m_x && m_y <= v.m_y);
	}

	bool Vector2::operator>=(Vector2 v)
	{
		return (m_x >= v.m_x && m_y >= v.m_y);
	}

	float Vector2::operator|(Vector2 v)
	{
		float Dot = 0;

		Dot = m_x * v.m_x + m_y * v.m_y;

		return Dot;
	}

	float Vector2::operator^(Vector2 v)
	{
		return m_x*v.m_y - m_y*v.m_x;
	}

	void Vector2::Normalize()
	{
		float Mag = Magnitude();
		m_x /= Mag;
		m_y /= Mag;
	}

	Vector2 Vector2::GetNormalized()
	{
		Vector2 Temp(INIT_NONE);
		Temp = *this;

		Temp.Normalize();
		return Temp;
	}


	float Vector2::Magnitude()
	{
		return Math::Sqrt((Math::Square(m_x) + Math::Square(m_y)));
	}

}