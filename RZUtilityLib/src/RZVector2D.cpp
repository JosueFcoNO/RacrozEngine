#pragma once
#include <RZUtilityPCH.h>
#include "RZVector2D.h"

namespace rczEngine {
	Vector2::Vector2(eInit init) noexcept : m_x(0.0f), m_y(0.0f)
	{
		if (init == eInit::Unit)
		{
			m_x = 1;
		}
	}

	Vector2::Vector2(const Vector2I& v2i) noexcept : m_x(gsl::narrow_cast<float>(v2i.m.x)), m_y(gsl::narrow_cast<float>(v2i.m.y)) {}
	Vector2::Vector2(const Vector3& v3) noexcept : m_x(v3.m_x), m_y(v3.m_y) {}
	Vector2::Vector2(const Vector4& v4) noexcept : m_x(v4.m_x), m_y(v4.m_y) {}

	Vector2& Vector2::operator+=(Vector2 v) noexcept
	{
		m_x += v.m_x;
		m_y += v.m_y;

		return *this;
	}

	Vector2& Vector2::operator-=(Vector2 v) noexcept
	{
		m_x -= v.m_x;
		m_y -= v.m_y;

		return *this;
	}



	Vector2& Vector2::operator*=(Vector2 v) noexcept
	{
		m_x *= v.m_x;
		m_y *= v.m_y;

		return *this;
	}

	Vector2& Vector2::operator*=(float f) noexcept
	{
		m_x *= f;
		m_y *= f;

		return *this;
	}

	Vector2& Vector2::operator/=(Vector2 v) noexcept
	{
		m_x /= v.m_x;
		m_y /= v.m_y;

		return *this;
	}

	Vector2& Vector2::operator/=(float f) noexcept
	{
		m_x /= f;
		m_y /= f;

		return *this;
	}

	void Vector2::Normalize() noexcept
	{
		const float Mag = Magnitude();
		m_x /= Mag;
		m_y /= Mag;
	}

	Vector2 Vector2::GetNormalized() noexcept
	{
		Vector2 Temp(*this);
		Temp.Normalize();
		return Temp;
	}

	float Vector2::Magnitude() noexcept
	{
		return Math::Sqrt((Math::Square(m_x) + Math::Square(m_y)));
	}

}