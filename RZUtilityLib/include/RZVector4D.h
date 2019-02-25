#pragma once


namespace rczEngine
{
	class RZ_EXP Vector4
	{
	public:
		///////////////////////
		////// Vector4 Constructors
		///////////////////////
		Vector4() noexcept : 
			m_x(0.0f), 
			m_y(0.0f), 
			m_z(0.0f), 
			m_w(0.0f) {};

		Vector4(eInit init) noexcept;

		Vector4(Vector3 v3) noexcept : 
			m_x(v3.m_x), 
			m_y(v3.m_y), 
			m_z(v3.m_z), 
			m_w(0.0f) {};

		Vector4(Vector2 v2) noexcept : 
			m_x(v2.m_x), 
			m_y(v2.m_y), 
			m_z(0.0f), 
			m_w(0.0f) {};

		Vector4(Vector2I v2i) noexcept : 
			m_x(gsl::narrow_cast<float>(v2i.m.x)), 
			m_y(gsl::narrow_cast<float>(v2i.m.y)), 
			m_z(0.0f), 
			m_w(0.0f) {};

		Vector4(const float& x, const float& y, const float& z, const float& w) noexcept : 
			m_x(x), 
			m_y(y), 
			m_z(z), 
			m_w(w) {}

		void Set(const float& x, const float& y, const float& z, const float& w) noexcept;
		void Scale(const float& Scale) noexcept;
		void Normalize() noexcept;
		Vector4 GetNormalized() const noexcept;
		FORCEINLINE float Magnitude() const noexcept;

		FORCEINLINE Vector4 operator+(const Vector4& v) const noexcept { return Vector4(*this) += v; }
		FORCEINLINE Vector4 operator-(const Vector4& v) const noexcept { return Vector4(*this) -= v; }
		FORCEINLINE Vector4 operator*(const Vector4& v) const noexcept { return Vector4(*this) *= v; }
		FORCEINLINE Vector4 operator/(const Vector4& v) const noexcept { return Vector4(*this) /= v; }

		FORCEINLINE Vector4 operator*(float f) const noexcept { return Vector4(*this) *= f; }
		FORCEINLINE Vector4 operator/(float f) const noexcept { return Vector4(*this) /= f; }

		Vector4& operator+=(const Vector4& v) noexcept;
		Vector4& operator-=(const Vector4& v) noexcept;
		Vector4& operator*=(const Vector4& v) noexcept;
		Vector4& operator/=(const Vector4& v) noexcept;

		Vector4& operator*=(float f) noexcept;
		Vector4& operator/=(float f) noexcept;

		FORCEINLINE bool operator==(const Vector4& v) const noexcept { return (m_x == v.m_x && m_y == v.m_y && m_z == v.m_z && m_w == v.m_w); }
		FORCEINLINE bool operator!=(const Vector4& v) const noexcept { return !(m_x == v.m_x && m_y == v.m_y && m_z == v.m_z && m_w != v.m_w); }
		FORCEINLINE bool operator<(const Vector4& v)  const noexcept { return (m_x < v.m_x && m_y < v.m_y && m_z < v.m_z && m_w < v.m_w); }
		FORCEINLINE bool operator>(const Vector4& v)  const noexcept { return (m_x > v.m_x && m_y > v.m_y && m_z > v.m_z && m_w > v.m_w); }
		FORCEINLINE bool operator<=(const Vector4& v) const noexcept { return (m_x <= v.m_x && m_y <= v.m_y && m_z <= v.m_z && m_w <= v.m_w); }
		FORCEINLINE bool operator>=(const Vector4& v) const noexcept { return (m_x >= v.m_x && m_y >= v.m_y && m_z >= v.m_z && m_w >= v.m_w); }

		FORCEINLINE float operator|(const Vector4& v) const noexcept { return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z + m_w * v.m_w; }
		FORCEINLINE float operator^(const Vector4& v) const noexcept { return m_x * v.m_y - m_y * v.m_x; }

		union
		{
			struct
			{
				float m_x, m_y, m_z, m_w;
			};
			float m_elements[4];
		};
	};

}