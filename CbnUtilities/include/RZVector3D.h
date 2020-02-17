#pragma once

namespace rczEngine 
{
	///////////////////////////
	/// A Vector3 float class.
	///////////////////////////
	//Addition
	//Substraction
	//Multiplication
	//Distance
	//Distance Squared
	//Angle XZ
	//Dot Product
	//Cross Product
	//Magnitude
	//Squared Magnitude
	//Projection
	//Projection Perpendicular
	//TripleProduct

	//Vector Identities
	//a+b = b+a						| Commutative addition
	//a-b = a + (-b) 1				| Vector Substraction definition
	//(a+b) + c = a + (b + c)		| Associative property of vector addition
	//s(ta) = (st)a					| Associative property of scalar multiplication
	//k(a+b) = ka + kb				| Scalar multiplications distributes over vector addition
	//||ka|| = |ka||a|				| Multiplying a vector by a scalar scales the magnitudes by the absolute value of the scalar
	//||a||							| The magnitude of a vector is nonnegative
	//||a||2 + ||b||2 = ||a + b||2	| The pythagorean theorem applied to vector addition
	//||a|| + ||b|| >= ||a + b||	| Triangle rule of vector addition. No side can be longer than the sum of the two other sides.
	//a|b = b|a						| Commutative property of dot product
	//||a|| = sqrt(a | a)			| Vector magnitude defined using dot product
	//k(a|b) = (ka)|b = a|(kb)		| Associative property of scalar multiplication with dot product.
	//a | (b + c) = a | b + a | c	| Dot product distributes over vector addition and substraction
	//a ^ a = 0						| The cross product of any vector with itself is the zero vector.
	//a ^ b = -(b ^ a)				| Cross product is anti- commutative
	//a ^ b = (-a) ^ (-b)			| Negating both operands produces the same vector
	//k(a^b)

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
		static Vector3 ProjectVector(const Vector3& one, const Vector3& two);
		static Vector3 ProjectVectorPerpendicular(const Vector3& one, const Vector3& two);
		static float DotProduct(const Vector3& dotLhs, const Vector3& dotRhs);
		static Vector3 CrossProduct(const Vector3& crossLhs, const Vector3& crossRhs);
		static float TripleProduct(const Vector3& dotV, const Vector3& crossA, const Vector3& crossB);

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