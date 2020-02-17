#pragma once
#include "CbnUtilitiesPCH.h"
#include "RZMatrix3D.h"

namespace rczEngine
{
	///////////////////////
	////// Matrix 3D
	///////////////////////

	Matrix3::Matrix3(eInit init) noexcept
	{
		if (init == eInit::None)
		{
			return;
		}
		else
			if (init == eInit::Unit)
			{
				*this = IDENTITY();
			}
			else
			{
				*this = ZERO();
			}
	}

	Matrix3::Matrix3(const Matrix4 & m4) noexcept
	{
		m_rows[0] = Vector3(m4.m_rows[0]);
		m_rows[1] = Vector3(m4.m_rows[1]);
		m_rows[2] = Vector3(m4.m_rows[2]);
	}

	Matrix3::Matrix3(const Vector3& top, const Vector3& mid, const Vector3& bottom) noexcept
	{
		m_rows[0] = top;
		m_rows[1] = mid;
		m_rows[2] = bottom;
	}

	float Matrix3::CoFactor2x2(uint32 nRow, uint32 nCol) const noexcept
	{
		///Create a 4 float array that will store the 2x2 matrix linearly.
		///Like So:
		/// [ A11 A12 ]
		/// [ A21 A22 ]
		float TrimedMatrix[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		uint8 TrimedMatrixCounter = 0;

		///Go through the matrix like Neo. 2 Nested Fors
		for (uint32 i = 0; i < 3; i++)
			for (uint32 j = 0; j < 3; j++)
			{
				///if my i equals nRow or my j equal nCol then continue the for, because that value is "trimmed" from the matrix.
				if (i == nRow || j == nCol)
				{
					continue;
				}

				///Store the value in TrimmedMatrix and increment the TrimedMatrixCounter.
				TrimedMatrix[TrimedMatrixCounter++] = m_matrix[i][j];

				///if i have all 4 values set on the TrimedMatrixCounter then break the for.
				if (TrimedMatrixCounter == 4)
				{
					break;
				}
			}

		///Return the Cofactor. 
		/// [ A11 A12 ]
		/// [ A21 A22 ]
		///	A11 * A22 - A12 * A21
		return TrimedMatrix[0] * TrimedMatrix[3] - TrimedMatrix[1] * TrimedMatrix[2];
	}

	void Matrix3::Transpose() noexcept
	{
		///Transpose the matrix
		Matrix3 R = *this;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m_matrix[j][i] = R.m_matrix[i][j];
	}

	void Matrix3::Identity() noexcept
	{
		for (int i = 0; i < 9; i++)
		{
			m_linear[i] = 0;
		}

		m_matrix[0][0] = 1.0f;
		m_matrix[1][1] = 1.0f;
		m_matrix[2][2] = 1.0f;
	}

	Matrix3 Matrix3::GetTransposed() const noexcept
	{
		///Return a copy of *this tranposed
		Matrix3 R = *this;


		R.Transpose();
		return R;
	}

	float Matrix3::Determinant() const noexcept
	{
		///Calculate the Determinant
		float Det = 0;

		Det = m_matrix[0][0] * m_matrix[1][1] * m_matrix[2][2] + m_matrix[0][1] * m_matrix[1][2] * m_matrix[2][0]
			+ m_matrix[0][2] * m_matrix[1][0] * m_matrix[2][1] - m_matrix[2][1] * m_matrix[1][2] * m_matrix[0][0]
			- m_matrix[2][2] * m_matrix[1][0] * m_matrix[0][1] - m_matrix[2][0] * m_matrix[1][1] * m_matrix[0][2];
		;

		return Det;
	}

	Matrix3 Matrix3::GetAdjoint() const noexcept
	{
		///Create an new matrix object
		Matrix3 Adjoint(eInit::None);

		///Form the Adjoint with the cofactors of the matrix. Adjoint[i][J] = CoFactor(i,j)
		for (uint32 i = 0; i < 3; i++)
			for (uint32 j = 0; j < 3; j++)
			{
				Adjoint.m_matrix[i][j] = Math::Pow(-1, gsl::narrow_cast<float>(i + j))*CoFactor2x2(i, j);
			}


		///Return the matrix Transposed and as an Adjoint
		return Adjoint.GetTransposed();
	}

	Matrix3 Matrix3::GetInverse() const noexcept
	{
		///Get the determinant
		const float Det = Determinant();

		///If it is 0 then return zero matrix.
		if (Det == 0)
		{
			return ZERO();
		}

		///Get the adjoint
		Matrix3 Adjoint = GetAdjoint();

		///Multiply it for 1/Det to get the Inverse Matrix
		Adjoint *= 1 / Det;

		///Return the Inverse Matrix
		return Adjoint;
	}

	Quaternion Matrix3::GetAsQuaternion() const noexcept
	{
		// Output quaternion
		float w = 0.0f, x = 0.0f, y = 0.0f, z = 0.0f;

		// Determine which of w, x, y, or z has the largest absolute value
		const float fourWSquaredMinus1 = m_elements.m00 + m_elements.m11 + m_elements.m22;
		const float fourXSquaredMinus1 = m_elements.m00 - m_elements.m11 - m_elements.m22;
		const float fourYSquaredMinus1 = m_elements.m11 - m_elements.m00 - m_elements.m22;
		const float fourZSquaredMinus1 = m_elements.m22 - m_elements.m00 - m_elements.m11;

		int biggestIndex = 0;

		float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
		if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}

		if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}
		if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		// Perform square root and division
		const float biggestVal = sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
		const float mult = 0.25f / biggestVal;

		// Apply table to compute quaternion values
		switch (biggestIndex) {
		case 0:
			w = biggestVal;
			x = (m_elements.m12 - m_elements.m21) * mult;
			y = (m_elements.m20 - m_elements.m02) * mult;
			z = (m_elements.m01 - m_elements.m10) * mult;
			break;
		case 1:
			x = biggestVal;
			w = (m_elements.m12 - m_elements.m21) * mult;
			y = (m_elements.m01 + m_elements.m10) * mult;
			z = (m_elements.m20 + m_elements.m02) * mult;
			break;
		case 2:
			y = biggestVal;
			w = (m_elements.m20 - m_elements.m02) * mult;
			x = (m_elements.m01 + m_elements.m10) * mult;
			z = (m_elements.m12 + m_elements.m21) * mult;
			break;
		case 3:
			z = biggestVal;
			w = (m_elements.m01 - m_elements.m10) * mult;
			x = (m_elements.m20 + m_elements.m02) * mult;
			y = (m_elements.m12 + m_elements.m21) * mult;
			break;
		}

		return Quaternion(x, y, z, w);
	}

	Matrix3 Matrix3::Translate2D(float xDelta, float yDelta) noexcept
	{
		///Create an Identity Matrix
		Matrix3 Temp(eInit::Unit);

		///Make it a Translation Matrix
		/// [ 1 0 xDelta ]
		/// [ 0 1 yDelta ]
		/// [ 0 0   1	 ]
		Temp.m_matrix[0][2] = xDelta;
		Temp.m_matrix[1][2] = yDelta;

		///Return Temp
		return Temp;
	}

	Matrix3 Matrix3::Scale2D(float xScale, float yScale) noexcept
	{
		///Create an Identity Matrix
		Matrix3 Temp(eInit::Unit);

		///Make it a Scale Matrix
		/// [ xScale 0		0 ]
		/// [ 0		yScale  0 ]
		/// [ 0		0		1 ]
		Temp.m_matrix[0][0] = xScale;
		Temp.m_matrix[1][1] = yScale;

		///Return Temp
		return Temp;
	}

	Matrix3 Matrix3::Rotate2D(Degree deg) noexcept
	{
		///Create a 2D rotation Matrix
		Matrix3 R(eInit::Unit);

		const float theta = deg.ValueRadian().Value();

		R.m_matrix[0][0] = R.m_matrix[1][1] = Math::Cos(theta);
		R.m_matrix[0][1] = Math::Sin(theta);
		R.m_matrix[1][0] = -Math::Sin(theta);

		return R;
	}

	Matrix3 Matrix3::ShearX2D(float xShear) noexcept
	{
		///Create an Identity Matrix
		Matrix3 Temp(eInit::Unit);

		///Make it a X Shear Matrix
		/// [ 1		0	xShear ]
		/// [ 0		1	  0	   ]
		/// [ 0		0	  1	   ]
		Temp.m_matrix[0][2] = xShear;

		///Return Temp
		return Temp;
	}

	Matrix3 Matrix3::ShearY2D(float yShear) noexcept
	{
		///Create an Identity Matrix
		Matrix3 Temp(eInit::Unit);

		///Make it a Y Shear Matrix
		/// [ 1		0	   0   ]
		/// [ 0		1	yShear ]
		/// [ 0		0	   1   ]
		Temp.m_matrix[1][2] = yShear;

		///Return Temp
		return Temp;
	}

	Matrix3 Matrix3::Rotate3D(Degree xRotation, Degree yRotation, Degree zRotation) noexcept
	{
		///Create a 3D rotation Matrix using the rczAXIS given
		Matrix3 x(eInit::Unit), y(eInit::Unit), z(eInit::Unit), fin(eInit::Unit);

		const float xtheta = xRotation.ValueRadian().Value();
		const float ytheta = yRotation.ValueRadian().Value();
		const float ztheta = zRotation.ValueRadian().Value();

		const int LeftHand = -1;

		if (xRotation != 0)
		{
			x.m_matrix[1][1] = x.m_matrix[2][2] = Math::Cos(xtheta);
			x.m_matrix[1][2] = Math::Sin(xtheta)*LeftHand;
			x.m_matrix[2][1] = -Math::Sin(xtheta)*LeftHand;
			fin *= x;
		}

		if (yRotation != 0)
		{
			y.m_matrix[0][0] = y.m_matrix[2][2] = Math::Cos(ytheta);
			y.m_matrix[0][2] = -Math::Sin(ytheta)*LeftHand;
			y.m_matrix[2][0] = Math::Sin(ytheta)*LeftHand;
			fin *= y;
		}

		if (zRotation != 0)
		{
			z.m_matrix[0][0] = z.m_matrix[1][1] = Math::Cos(ztheta);
			z.m_matrix[0][1] = Math::Sin(ztheta)*LeftHand;
			z.m_matrix[1][0] = -Math::Sin(ztheta)*LeftHand;
			fin *= z;
		}

		return fin;
	}

	Matrix3 Matrix3::operator*(const float& scalar) const noexcept
	{
		Matrix3 Temp = *this;

		for (int i = 0; i < 9; i++)
		{
			Temp.m_linear[i] *= scalar;
		}

		return Temp;
	}

	Matrix3 Matrix3::operator* (const Matrix3& M) const noexcept
	{
		const Matrix3 Temp = M.GetTransposed();
		const Matrix3 This = *this;
		Matrix3 TempReturn(eInit::None);

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				TempReturn.m_matrix[i][j] = This.m_rows[i] | Temp.m_rows[j];
			}

		return TempReturn;
	}

	Vector3 Matrix3::operator* (const Vector3& V) const noexcept
	{
		Vector3 transformedVector(eInit::None);

#pragma omp simd
		for (int32 i = 0; i < 3; ++i)
		{
			transformedVector.m_elements[i] = m_rows[i] | V;
		}

		return transformedVector;
	}

	Matrix3 Matrix3::operator+(const Matrix3 & M) const noexcept
	{
		Matrix3 sum(eInit::None);
		sum.m_rows[0] = m_rows[0] + M.m_rows[0];
		sum.m_rows[1] = m_rows[1] + M.m_rows[1];
		sum.m_rows[2] = m_rows[2] + M.m_rows[2];

		return sum;
	}

	Matrix3 Matrix3::operator-(const Matrix3 & M) const noexcept
	{
		Matrix3 subs(eInit::None);
		subs.m_rows[0] = m_rows[0] - M.m_rows[0];
		subs.m_rows[1] = m_rows[1] - M.m_rows[1];
		subs.m_rows[2] = m_rows[2] - M.m_rows[2];

		return subs;
	}

	void Matrix3::operator*=(const float& scalar) noexcept
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				m_matrix[i][j] *= scalar;
			}

	}

	void Matrix3::operator*=(const Matrix3& M) noexcept
	{
		const Matrix3 Temp = *this;

		*this = Temp * M;
	}

	Matrix3& Matrix3::operator+=(const Matrix3 & M) noexcept
	{
		m_rows[0] = m_rows[0] + M.m_rows[0];
		m_rows[1] = m_rows[1] + M.m_rows[1];
		m_rows[2] = m_rows[2] + M.m_rows[2];

		return *this;
	}

	Matrix3 & Matrix3::operator-=(const Matrix3 & M) noexcept
	{
		m_rows[0] = m_rows[0] + M.m_rows[0];
		m_rows[1] = m_rows[1] + M.m_rows[1];
		m_rows[2] = m_rows[2] + M.m_rows[2];

		return *this;
	}
}