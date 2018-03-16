#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Vector2I
	{
	public:
		///////////////////////
		////// Vector2I Constructors
		///////////////////////

		Vector2I() {};

		Vector2I(eINIT init);

		Vector2I(Vector2& v2);
		Vector2I(Vector3& v3);
		Vector2I(Vector4& v4);

		Vector2I(int x, int y);

		void Set(int x, int y);
		void Scale(float Scale);

		Vector2I operator+(const Vector2I& v);
		Vector2I operator-(const Vector2I& v);
		Vector2I operator*(const Vector2I& v);
		Vector2I operator*(float f);
		Vector2I operator/(const Vector2I& v);
		Vector2I operator/(float f);

		Vector2I& operator+=(const Vector2I& v);
		Vector2I& operator-=(const Vector2I& v);
		Vector2I& operator*=(const Vector2I& v);
		Vector2I& operator*=(float f);
		Vector2I& operator/=(const Vector2I& v);
		Vector2I& operator/=(float f);

		bool operator==(const Vector2I& v);
		bool operator!=(const Vector2I& v);
		bool  operator<(const Vector2I& v);
		bool  operator>(const Vector2I& v);
		bool operator<=(const Vector2I& v);
		bool operator>=(const Vector2I& v);

		float operator|(const Vector2I& v);
		float operator^(const Vector2I& v);

		void Normalize();
		Vector2I GetNormalized();

		float Magnitude();

		int m_x, m_y;
	};
}