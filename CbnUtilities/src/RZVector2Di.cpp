#pragma once
#include "CbnUtilitiesPCH.h"
#include "RZVector2Di.h"

namespace rczEngine
{

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	Vector2I::Vector2I(eInit init) noexcept
	{
		if (init == eInit::None)
		{
			return;
		}
		else if (init == eInit::Unit)
		{
			m.x = 1;
			m.y = 0;
		}
		else
		{
			m.x = 0;
			m.y = 0;
		}
	}

	Vector2I::Vector2I(Vector2& v2) noexcept
	{
		m.x = Math::Truncate(v2.m_x);
		m.y = Math::Truncate(v2.m_y);
	}

	Vector2I::Vector2I(Vector3& v3) noexcept
	{
		m.x = Math::Truncate(v3.m_x);
		m.y = Math::Truncate(v3.m_y);
	}

	Vector2I::Vector2I(Vector4& v4) noexcept
	{
		m.x = Math::Truncate(v4.m_x);
		m.y = Math::Truncate(v4.m_y);
	}

	Vector2I::Vector2I(int x, int y) noexcept
	{
		m.x = x;
		m.y = y;
	}

	Vector2I& Vector2I::operator+=(const Vector2I& v) noexcept
	{
		m.x += v.m.x;
		m.y += v.m.y;

		return *this;
	}

	Vector2I& Vector2I::operator-=(const Vector2I& v) noexcept
	{
		m.x -= v.m.x;
		m.y -= v.m.y;

		return *this;
	}

	Vector2I& Vector2I::operator*=(const Vector2I& v) noexcept
	{
		m.x *= v.m.x;
		m.y *= v.m.y;

		return *this;
	}

	Vector2I& Vector2I::operator*=(float f) noexcept
	{
		m.x *= Math::Truncate(f);
		m.y *= Math::Truncate(f);

		return *this;
	}

	Vector2I& Vector2I::operator/=(const Vector2I& v) noexcept
	{
		m.x /= v.m.x;
		m.y /= v.m.y;

		return *this;
	}

	Vector2I& Vector2I::operator/=(float f) noexcept
	{
		m.x /= Math::Truncate(f);
		m.y /= Math::Truncate(f);

		return *this;
	}

	float Vector2I::Magnitude() const noexcept
	{
		return Math::Sqrt(gsl::narrow_cast<float>(Math::Square(m.x) + Math::Square(m.y)));
	}

	void Vector2I::Normalize() noexcept
	{
		const float Mag = Magnitude();
		m.x /= Math::Truncate(Mag);
		m.y /= Math::Truncate(Mag);
	}

	Vector2I Vector2I::GetNormalized() const noexcept
	{
		Vector2I Temp(*this);
		Temp.Normalize();
		return Temp;
	}

}