#pragma once
#include <RZUtilityPCH.h>
#include "RZVector2Di.h"

namespace rczEngine
{

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	Vector2I::Vector2I(eINIT init)
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

	Vector2I::Vector2I(Vector2& v2)
	{
		m_x = (int32)v2.m_x;
		m_y = (int32)v2.m_y;
	}

	Vector2I::Vector2I(Vector3& v3)
	{
		m_x = (int32)v3.m_x;
		m_y = (int32)v3.m_y;
	}

	Vector2I::Vector2I(Vector4& v4)
	{
		m_x = (int32)v4.m_x;
		m_y = (int32)v4.m_y;
	}

	Vector2I::Vector2I(int x, int y)
	{
		m_x = x;
		m_y = y;
	}

	void Vector2I::Set(int x, int y)
	{
		m_x = x;
		m_y = y;
	}

	void Vector2I::Scale(float Scale)
	{
		m_x *= Math::Truncate(Scale);
		m_y *= Math::Truncate(Scale);
	}

	Vector2I Vector2I::operator+(const Vector2I& v)
	{
		return Vector2I(m_x + v.m_x, m_y + v.m_y);
	}

	Vector2I Vector2I::operator-(const Vector2I& v)
	{
		return Vector2I(m_x - v.m_x, m_y - v.m_y);
	}

	Vector2I Vector2I::operator*(const Vector2I& v)
	{
		return Vector2I(m_x * v.m_x, m_y * v.m_y);
	}

	Vector2I Vector2I::operator*(float f)
	{
		return Vector2I(Math::Truncate(m_x * f), Math::Truncate(m_y * f));
	}

	Vector2I Vector2I::operator/(const Vector2I& v)
	{
		return Vector2I(m_x / v.m_x, m_y / v.m_y);
	}

	Vector2I Vector2I::operator/(float f)
	{
		return Vector2I(Math::Truncate(m_x / f), Math::Truncate(m_y / f));
	}

	Vector2I& Vector2I::operator+=(const Vector2I& v)
	{
		m_x += v.m_x;
		m_y += v.m_y;

		return *this;
	}

	Vector2I& Vector2I::operator-=(const Vector2I& v)
	{
		m_x -= v.m_x;
		m_y -= v.m_y;

		return *this;
	}



	Vector2I& Vector2I::operator*=(const Vector2I& v)
	{
		m_x *= v.m_x;
		m_y *= v.m_y;

		return *this;
	}

	Vector2I& Vector2I::operator*=(float f)
	{
		m_x *= Math::Truncate(f);
		m_y *= Math::Truncate(f);

		return *this;
	}

	Vector2I& Vector2I::operator/=(const Vector2I& v)
	{
		m_x /= v.m_x;
		m_y /= v.m_y;

		return *this;
	}

	Vector2I& Vector2I::operator/=(float f)
	{
		m_x /= Math::Truncate(f);
		m_y /= Math::Truncate(f);

		return *this;
	}

	bool Vector2I::operator==(const Vector2I& v)
	{
		return (m_x == v.m_x && m_y == v.m_y);
	}

	bool Vector2I::operator!=(const Vector2I& v)
	{
		return !(m_x == v.m_x && m_y == v.m_y);
	}

	bool Vector2I::operator<(const Vector2I& v)
	{
		return (m_x < v.m_x && m_y < v.m_y);
	}

	bool Vector2I::operator>(const Vector2I& v)
	{
		return (m_x > v.m_x && m_y > v.m_y);
	}

	bool Vector2I::operator<=(const Vector2I& v)
	{
		return (m_x <= v.m_x && m_y <= v.m_y);
	}

	bool Vector2I::operator>=(const Vector2I& v)
	{
		return (m_x >= v.m_x && m_y >= v.m_y);
	}

	float Vector2I::operator|(const Vector2I& v)
	{
		float Dot = 0;

		Dot = float(m_x * v.m_x + m_y * v.m_y);

		return Dot;
	}

	float Vector2I::operator^(const Vector2I& v)
	{
		return float(m_x*v.m_y - m_y*v.m_x);
	}

	void Vector2I::Normalize()
	{
		float Mag = Magnitude();
		m_x /= Math::Truncate(Mag);
		m_y /= Math::Truncate(Mag);
	}

	Vector2I Vector2I::GetNormalized()
	{
		Vector2I Temp(INIT_NONE);
		Temp = *this;

		Temp.Normalize();
		return Temp;
	}


	float Vector2I::Magnitude()
	{
		return Math::Sqrt((float)(Math::Square(m_x) + Math::Square(m_y)));
	}
}