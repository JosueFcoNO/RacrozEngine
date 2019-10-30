#pragma once

namespace rczEngine
{
	///4x4 Matrix that can be accesed as 4 row Vector4s, a linear 16 float array and a 4x4 float nested array.
	class RZ_EXP Matrix4
	{
	public:
		///////////////////////
		////// Matrix4 Constructors
		///////////////////////

		Matrix4() noexcept : m_rows{ Vector4(), Vector4(), Vector4(), Vector4() } {};

		///Inits the matrix with either an Identity Matrix or a Zero Matrix
		Matrix4(eInit init) noexcept;

		///Inits the matrix with another 3x3 matrix, the last row is added as (0,0,0,1)
		Matrix4(const Matrix3& matrix) noexcept;

		Matrix4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		) noexcept;

		///Vector4 Constructor. Receives Vector4s as its 4 rows.
		Matrix4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) noexcept;

		///////////////////////
		////// Matrix Operations
		///////////////////////

		///Returns the cofactor of the nRow and nCol from this Matrix
		float CoFactor3x3(uint32 nRow, uint32 nCol) const noexcept;
		///Transposes the Matrix
		void Transpose() noexcept;
		///Makes it an Identity Matrix
		void Identity() noexcept;
		///Returns a copy of this Matrix Transposed
		Matrix4 GetTransposed() const noexcept;
		///Returns the Determinant of this Matrix
		float Determinant() const;
		///Returns the Adjoint of this Matrix
		Matrix4 GetAdjoint() const;
		///returns the Inverse of this Matrix
		Matrix4 GetInverse() const;

		///Returns a Quaternion made from this matrix.
		Quaternion GetAsQuaternion();

		///////////////////////
		////// 3D Graphics Operations
		///////////////////////

		static Matrix4 Translate3D(float xDelta, float yDelta, float zDelta);
		static Matrix4 Scale3D(float xScale, float yScale, float zScale);
		static Matrix4 Rotate3D(Degree xRotation, Degree yRotation, Degree zRotation);
		static Matrix4 OrthoProjectedSpace(float width, float height, float ZNear, float ZFar);
		static Matrix4 PerpsProjectedSpace(float FovY, float AspectRatio, float ZNear, float ZFar) noexcept;
		static Matrix4 LookAtMatrix(Vector3 eye, Vector3 up, Vector3 target) noexcept;
		static Matrix4 LookAtMatrixZ(Vector3 eye, Vector3 up, Vector3 target) noexcept;
		static Matrix4 LookAtMatrixY(Vector3 eye, Vector3 up, Vector3 target) noexcept;

		///////////////////////
		////// Aritmethic Operations
		///////////////////////

		///Multiplies the matrix and scalar. Constant.
		Matrix4 operator*(const float& scalar) const noexcept;
		///Multiplies the matrix and another 4x4 Matrix. Constant.
		Matrix4 operator* (const Matrix4& M) const noexcept;
		///Multiplies the matrix and a Vector4: Vector * Matrix so it returns a Vector4 . Constant.
		Vector4 operator* (const Vector4& V) const noexcept;
		///Multiplies the matrix and a Vector3: Vector * Matrix so it returns a Vector3 . Constant.
		Vector3 operator* (const Vector3& V) const noexcept;

		///Multiplies the matrix and scalar.
		void operator*=(float scalar) noexcept;
		///Multiplies the matrix and another 4x4 Matrix.
		void operator*=(Matrix4 M) noexcept;

		union {
			Vector4 m_rows[4];
			float m_matrix[4][4];
			float m_linear[16];
			struct {
				float m00, m01, m02, m03,
					m10, m11, m12, m13,
					m20, m21, m22, m23,
					m30, m31, m32, m33;
			} m_elements;
		};

		static const Matrix4& IDENTITY() noexcept { static Matrix4 Identity(
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		); return Identity; };
		
		static const Matrix4& ZERO() noexcept { static Matrix4 Zero(
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f }
		); return Zero; };
	};
}