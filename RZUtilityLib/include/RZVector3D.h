#pragma once

namespace rczEngine {

	class RZ_UTILITY_EXPORT Vector3
	{
	public:
		///////////////////////
		////// Vector3 Constructors
		///////////////////////

		Vector3() {};

		Vector3(eINIT init);

		Vector3(const Vector2& v2);
		Vector3(const Vector2I& v2i);
		Vector3(const Vector4& v4);

		Vector3(const float& x, const float& y, const float& z);

		void Set(const float& x, const float& y, const float& z);
		void Set(const Vector3& v);
		void Scale(const float& Scale);
		static float Distance(const Vector3& v1, const Vector3& v2);
		static float Anglexz(const Vector3& v, bool getAsDegree);


		Vector3 operator+(const Vector3& v) const;
		Vector3 operator-(const Vector3& v) const;
		Vector3 operator*(const Vector3& v) const;
		Vector3 operator*(const float& f) const;
		Vector3 operator/(const Vector3& v) const;
		Vector3 operator/(const float& f) const;

		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(const Vector3& v);
		Vector3& operator*=(const float& f);
		Vector3& operator/=(const Vector3& v);
		Vector3& operator/=(const float& f);

		bool operator==(const Vector3& v) const;
		bool operator!=(const Vector3& v) const;
		bool operator<(const Vector3& v) const;
		bool operator>(const Vector3& v) const ;
		bool operator<=(const Vector3& v) const;
		bool operator>=(const Vector3& v) const;

		float operator|(const Vector3& v) const;
		Vector3 operator^(const Vector3& v) const;

		void Normalize();
		Vector3 GetNormalized();


		float Magnitude();


		float m_x, m_y, m_z;

		static const Vector3 ZERO;

	};

}