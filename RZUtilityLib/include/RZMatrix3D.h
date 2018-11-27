#pragma once

namespace rczEngine
{
	///3x3 Matrix that can be accesed as 3 row Vector3s, a linear 9 float array and a 3x3 float nested array.
	class RZ_EXP Matrix3
	{
	public:
		///////////////////////
		////// Matrix3 Constructors
		///////////////////////
		Matrix3() {};

		///Int Constructor, can INIT_ZERO and INIT_ONE
		Matrix3(eINIT init);
		///Constructs a Matrix3 from a Matrix4
		Matrix3(const Matrix4& m4);
		///Vector Constructor: Takes 3 Vector3. Top, Middle and Bottom Row. 
		Matrix3(const Vector3& top, const Vector3& mid, const Vector3& bottom);

		Matrix3(const Matrix3& other)
		{
			m_elements = other.m_elements;
		};

		Matrix3& operator=(const Matrix3& other) 
		{ 
			m_elements = other.m_elements; 
			return *this;
		};

		///////////////////////
		////// Matrix Operations
		///////////////////////

		///Returns the cofactor of the nRow and nCol from this Matrix
		float CoFactor2x2(uint32 nRow, uint32 nCol) const;
		///Transposes this Matrix
		void Transpose();
		///Makes it an Identity Matrix
		void Identity();
		///Returns a copy of this Matrix Transposed
		Matrix3 GetTransposed() const;
		///Returns the Determinant of this Matrix
		float Determinant() const;
		///Returns the Adjoint of this Matrix
		Matrix3 GetAdjoint() const;
		///Returns the inverse of this Matrix
		Matrix3 GetInverse() const;

		Quaternion GetAsQuaternion();

		///////////////////////
		////// 2D Graphics Operations
		///////////////////////

		static Matrix3 Translate2D(float xDelta, float yDelta);
		static Matrix3 Scale2D(float xScale, float yScale);
		static Matrix3 Rotate2D(Degree deg);
		static Matrix3 ShearX2D(float xShear);
		static Matrix3 ShearY2D(float yShear);

		///////////////////////
		////// 3D Graphics Operations
		///////////////////////

		static Matrix3 Rotate3D(Degree xRotation, Degree yRotation, Degree zRotation);

		///////////////////////
		////// Aritmethic Operations
		///////////////////////

		///Multiplies the matrix and scalar. Constant.
		Matrix3 operator*(const float& scalar) const;
		///Multiplies the matrix and another 3x3 Matrix. Constant.
		Matrix3 operator* (const Matrix3& M) const;
		///Multiplies the matrix and a Vector3: Vector * Matrix so it returns a Vector3 . Constant.
		Vector3 operator* (const Vector3& V) const;
		///Adds the matrices
		Matrix3 operator+(const Matrix3& M) const;
		///Substracts the matrices
		Matrix3 operator-(const Matrix3& M) const;

		///Multiplies the Matrix and a Scalar.
		void operator*=(const float& scalar);
		///Multiplies the Matrix and another Matrix.
		void operator*=(const Matrix3& M);
		///Adds the matrices
		Matrix3& operator+=(const Matrix3& M);
		///Substracts the matrices
		Matrix3& operator-=(const Matrix3& M);


		union {
			Vector3 m_rows[3];
			float m_matrix[3][3];
			float m_linear[9];
			struct {
				float m00, m01, m02,
					m10, m11, m12,
					m20, m21, m22;
			} m_elements;
		};

		static const Matrix3 IDENTITY;
		static const Matrix3 ZERO;
	};


};
