#pragma once

namespace rczEngine
{
	class RZ_EXP Vector2I
	{
	public:
		///////////////////////
		////// Vector2I Constructors
		///////////////////////

		Vector2I() noexcept {};

		Vector2I(eInit init) noexcept;

		Vector2I(Vector2& v2) noexcept;
		Vector2I(Vector3& v3) noexcept;
		Vector2I(Vector4& v4) noexcept;

		Vector2I(int x, int y) noexcept;

		FORCEINLINE void Set(int x, int y) noexcept { m.x = x; m.y = y; }
		FORCEINLINE void Scale(float scale) noexcept { *this *= scale; }
		FORCEINLINE float Magnitude() const noexcept;
		void Normalize() noexcept;
		Vector2I GetNormalized() const noexcept;

		FORCEINLINE Vector2I operator+(const Vector2I& v) noexcept { return Vector2I(m.x, m.y) += v; }
		FORCEINLINE Vector2I operator-(const Vector2I& v) noexcept { return Vector2I(m.x, m.y) -= v; }
		FORCEINLINE Vector2I operator*(const Vector2I& v) noexcept { return Vector2I(m.x, m.y) *= v; }
		FORCEINLINE Vector2I operator/(const Vector2I& v) noexcept { return Vector2I(m.x, m.y) /= v; }

		FORCEINLINE Vector2I operator*(float f) noexcept { return Vector2I(m.x, m.y) *= f; }
		FORCEINLINE Vector2I operator/(float f) noexcept { return Vector2I(m.x, m.y) /= f; }

		Vector2I& operator+=(const Vector2I& v) noexcept;
		Vector2I& operator-=(const Vector2I& v) noexcept;
		Vector2I& operator*=(const Vector2I& v) noexcept;
		Vector2I& operator/=(const Vector2I& v) noexcept;

		Vector2I& operator*=(float f) noexcept;
		Vector2I& operator/=(float f) noexcept;

		FORCEINLINE bool operator==(const Vector2I& v) const noexcept { return (m.x == v.m.x && m.y == v.m.y); }
		FORCEINLINE bool operator!=(const Vector2I& v) const noexcept { return !(m.x == v.m.x && m.y == v.m.y); }
		FORCEINLINE bool  operator<(const Vector2I& v) const noexcept { return (m.x < v.m.x && m.y < v.m.y); }
		FORCEINLINE bool  operator>(const Vector2I& v) const noexcept { return (m.x > v.m.x && m.y > v.m.y); }
		FORCEINLINE bool operator<=(const Vector2I& v) const noexcept { return (m.x <= v.m.x && m.y <= v.m.y); }
		FORCEINLINE bool operator>=(const Vector2I& v) const noexcept { return (m.x >= v.m.x && m.y >= v.m.y); }

		FORCEINLINE float operator|(const Vector2I& v) const noexcept { return gsl::narrow_cast<float>(m.x * v.m.x + m.y * v.m.y); }
		FORCEINLINE float operator^(const Vector2I& v) const noexcept { return gsl::narrow_cast<float>(m.x*v.m.y - m.y * v.m.x); }

		union
		{
			struct
			{
				int x, y;
			} m;
			int m_elements[2];
		};
	};
}