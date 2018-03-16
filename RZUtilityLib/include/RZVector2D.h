#pragma once

namespace rczEngine {

	class RZ_UTILITY_EXPORT Vector2
	{
	public:
		///////////////////////
		////// Vector2 Constructors
		///////////////////////

		Vector2() {};

		Vector2(eINIT init);

		Vector2(const Vector2I& v2i);
		Vector2(const Vector3& v3);
		Vector2(const Vector4& v4);

		Vector2(float x, float y);

		void Set(float x, float y);
		void Scale(float Scale);

		Vector2 operator+(const Vector2& v) const;
		Vector2 operator-(const Vector2& v) const;
		Vector2 operator*(const Vector2& v) const;
		Vector2 operator*(const float& f) const;
		Vector2 operator/(const Vector2& v) const;
		Vector2 operator/(const float& f) const;

		Vector2& operator+=(Vector2 v);
		Vector2& operator-=(Vector2 v);
		Vector2& operator*=(Vector2 v);
		Vector2& operator*=(float f);
		Vector2& operator/=(Vector2 v);
		Vector2& operator/=(float f);

		bool operator==(Vector2 v);
		bool operator!=(Vector2 v);
		bool operator<(Vector2 v);
		bool operator>(Vector2 v);
		bool operator<=(Vector2 v);
		bool operator>=(Vector2 v);

		float operator|(Vector2 v);
		float operator^(Vector2 v);

		void Normalize();
		Vector2 GetNormalized();


		float Magnitude();


		float m_x, m_y;

	};

}