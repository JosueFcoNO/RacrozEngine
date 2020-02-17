#pragma once

namespace rczEngine {

	class RZ_EXP Vector2
	{
	public:
		///////////////////////
		////// Vector2 Constructors
		///////////////////////

		Vector2() noexcept : m_x(0.0f), m_y(0.0f) {};

		Vector2(eInit init) noexcept;
		Vector2(const Vector2I& v2i) noexcept;
		Vector2(const Vector3& v3) noexcept;
		Vector2(const Vector4& v4) noexcept;
		Vector2(float x, float y) noexcept : m_x(x), m_y(y) {};

		FORCEINLINE void Set(float x, float y) noexcept { m_x = x; m_y = y; }
		FORCEINLINE void Scale(float scale) noexcept { *this *= scale; }

		FORCEINLINE Vector2 operator+(const Vector2& v) const noexcept { return Vector2{ m_x, m_y } += v; }
		FORCEINLINE Vector2 operator-(const Vector2& v) const noexcept { return Vector2{ m_x, m_y } -= v; }
		FORCEINLINE Vector2 operator*(const Vector2& v) const noexcept { return Vector2{ m_x, m_y } *= v; }
		FORCEINLINE Vector2 operator/(const Vector2& v) const noexcept { return Vector2{ m_x, m_y } /= v; }
		
		FORCEINLINE Vector2 operator*(const float& f) const noexcept { return Vector2{ m_x, m_y } *= f; }
		FORCEINLINE Vector2 operator/(const float& f) const noexcept { return Vector2{ m_x, m_y } /= f; }

		Vector2& operator+=(Vector2 v) noexcept;
		Vector2& operator-=(Vector2 v) noexcept;
		Vector2& operator*=(Vector2 v) noexcept;
		Vector2& operator*=(float f) noexcept;
		Vector2& operator/=(Vector2 v) noexcept;
		Vector2& operator/=(float f) noexcept;

		FORCEINLINE bool operator==(Vector2 v) noexcept { return !(m_x == v.m_x && m_y == v.m_y); }
		FORCEINLINE bool operator!=(Vector2 v) noexcept { return (m_x == v.m_x && m_y == v.m_y); }
		FORCEINLINE bool operator<(Vector2 v) noexcept { return (m_x < v.m_x && m_y < v.m_y); }
		FORCEINLINE bool operator>(Vector2 v) noexcept { return (m_x > v.m_x && m_y > v.m_y); }
		FORCEINLINE bool operator<=(Vector2 v) noexcept { return (m_x <= v.m_x && m_y <= v.m_y); }
		FORCEINLINE bool operator>=(Vector2 v) noexcept { return (m_x >= v.m_x && m_y >= v.m_y); }

		float operator|(Vector2 v) noexcept { return m_x * v.m_x + m_y * v.m_y; }
		float operator^(Vector2 v) noexcept { return m_x * v.m_y - m_y * v.m_x; }

		void Normalize() noexcept;
		Vector2 GetNormalized() noexcept;

		float Magnitude() noexcept;
		float MagnitudeSqr() noexcept;

		union
		{
			struct
			{
				float m_x, m_y;
			};
			float m_elements[2];
		};
	};

}