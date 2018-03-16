#pragma once


namespace rczEngine
{
	class RZ_UTILITY_EXPORT Vector4
	{
	public:
		///////////////////////
		////// Vector4 Constructors
		///////////////////////

		Vector4() {};

		Vector4(eINIT init);

		Vector4(Vector3 v3);
		Vector4(Vector2 v2);
		Vector4(Vector2I v2i);

		Vector4(const float& x, const float& y, const float& z, const float& w);

		void Set(const float& x, const float& y, const float& z, const float& w);
		void Scale(const float& Scale);

		Vector4 operator+(const Vector4& v) const;
		Vector4 operator-(const Vector4& v) const;
		Vector4 operator*(const Vector4& v) const;
		Vector4 operator*(float f);
		Vector4 operator/(const Vector4& v) const;
		Vector4 operator/(float f);

		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(const Vector4& v);
		Vector4& operator*=(float f);
		Vector4& operator/=(const Vector4& v);
		Vector4& operator/=(float f);

		bool operator==(const Vector4& v) const;
		bool operator!=(const Vector4& v) const;
		bool operator<(const Vector4& v) const;
		bool operator>(const Vector4& v) const;
		bool operator<=(const Vector4& v) const;
		bool operator>=(const Vector4& v) const;

		float operator|(const Vector4& v) const;
		float operator^(const Vector4& v) const;

		void Normalize();
		Vector4 GetNormalized();

		float Magnitude();

		static const Vector4 ZERO;

		float m_x, m_y, m_z, m_w;

	};

}