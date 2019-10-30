#pragma once

namespace rczEngine {

	class RZ_EXP Vector3
	{
	public:
		///////////////////////
		////// Vector3 Constructors
		///////////////////////

		Vector3() noexcept :
			m_x(0.0f),
			m_y(0.0f),
			m_z(0.0f) {};

		Vector3(eInit init) noexcept;

		Vector3(const Vector2& v2) noexcept :
			m_x(v2.m_x),
			m_y(v2.m_y),
			m_z(0.0f) {};

		Vector3(const Vector2I& v2i) noexcept :
			m_x(gsl::narrow_cast<float>(v2i.m.x)),
			m_y(gsl::narrow_cast<float>(v2i.m.y)),
			m_z(0.0f) {};

		Vector3(const Vector4& v4) noexcept;

		Vector3(float x, float y, float z) noexcept :
			m_x(x),
			m_y(y),
			m_z(z) {};

		static float Distance(const Vector3& vhead, const Vector3& vtail) noexcept;
		static float SqrDistance(const Vector3 & vhead, const Vector3 & vtail) noexcept;
		static float Anglexz(const Vector3& v, bool getAsDegree) noexcept;
		static uint32 Hash(const Vector3& v, int mult = 10000) noexcept;

		void Set(const float& x, const float& y, const float& z) noexcept;
		void Set(const Vector3& v) noexcept;
		void Scale(const float& Scale) noexcept;
		void Normalize() noexcept;
		Vector3 GetNormalized() const noexcept;
		FORCEINLINE float Magnitude() const noexcept;
		FORCEINLINE float SqrMagnitude() const noexcept;

		Vector3 operator+(const Vector3& v) const noexcept;
		Vector3 operator-(const Vector3& v) const noexcept;
		Vector3 operator*(const Vector3& v) const noexcept;
		Vector3 operator/(const Vector3& v) const noexcept;

		Vector3 operator*(float f) const noexcept;
		Vector3 operator/(float f) const noexcept;

		Vector3& operator+=(const Vector3& v) noexcept;
		Vector3& operator-=(const Vector3& v) noexcept;
		Vector3& operator*=(const Vector3& v) noexcept;
		Vector3& operator/=(const Vector3& v) noexcept;

		Vector3& operator*=(const float& f) noexcept;
		Vector3& operator/=(const float& f) noexcept;

		FORCEINLINE bool operator==(const Vector3& v) const noexcept { return (m_x == v.m_x && m_y == v.m_y && m_z == v.m_z); }
		FORCEINLINE bool operator!=(const Vector3& v) const noexcept { return !(m_x == v.m_x && m_y == v.m_y && m_z == v.m_z); }
		FORCEINLINE bool operator<(const Vector3& v)  const noexcept { return (m_x < v.m_x && m_y < v.m_y && m_z < v.m_z); }
		FORCEINLINE bool operator>(const Vector3& v)  const noexcept { return (m_x > v.m_x && m_y > v.m_y && m_z > v.m_z); }
		FORCEINLINE bool operator<=(const Vector3& v) const noexcept { return (m_x <= v.m_x && m_y <= v.m_y && m_z <= v.m_z); }
		FORCEINLINE bool operator>=(const Vector3& v) const noexcept { return (m_x >= v.m_x && m_y >= v.m_y && m_z >= v.m_z); }

		FORCEINLINE float operator|(const Vector3& v) const noexcept { return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z; }
		FORCEINLINE Vector3 operator^(const Vector3& v) const noexcept;

		union
		{
			struct
			{
				float m_x;
				float m_y;
				float m_z;
			};
			float m_elements[3];
		};
	};

}