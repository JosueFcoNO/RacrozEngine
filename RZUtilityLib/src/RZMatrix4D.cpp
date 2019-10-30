#pragma once
#include <RZUtilityPCH.h>
#include "RZMatrix4D.h"

namespace rczEngine
{
	////////////////////////////////////
	/////////Matrix 4D
	////////////////////////////////////

	Matrix4::Matrix4(eInit init) noexcept
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

	Matrix4::Matrix4(const Matrix3& matrix) noexcept
	{
		m_rows[0] = Vector4(matrix.m_rows[0]);
		m_rows[1] = Vector4(matrix.m_rows[1]);
		m_rows[2] = Vector4(matrix.m_rows[2]);
		m_rows[3] = Vector4(0, 0, 0, 1);
	}

	Matrix4::Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) noexcept
	{
		m_elements.m00 = m00;
		m_elements.m01 = m01;
		m_elements.m02 = m02;
		m_elements.m03 = m03;

		m_elements.m10 = m10;
		m_elements.m11 = m11;
		m_elements.m12 = m12;
		m_elements.m13 = m13;

		m_elements.m20 = m20;
		m_elements.m21 = m21;
		m_elements.m22 = m22;
		m_elements.m23 = m23;

		m_elements.m30 = m30;
		m_elements.m31 = m31;
		m_elements.m32 = m32;
		m_elements.m33 = m33;
	}

	Matrix4::Matrix4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4) noexcept
	{
		m_rows[0] = v1;
		m_rows[1] = v2;
		m_rows[2] = v3;
		m_rows[3] = v4;
	}

	float Matrix4::CoFactor3x3(uint32 nRow, uint32 nCol) const noexcept
	{
		///Create a 3x3 matrix.
		///Like So:
		/// [ A11 A12 A13 ]
		/// [ A21 A22 A23 ]
		/// [ A31 A32 A33 ]
		Matrix3 TrimedMatrix(eInit::Zero);
		uint8 TrimedMatrixCounter = 0;

		///Go through the matrix like Neo. 2 Nested Fors
		for (uint32 i = 0; i < 4; i++)
			for (uint32 j = 0; j < 4; j++)
			{
				///if my i equals nRow or my j equal nCol then continue the for, because that value is "trimmed" from the matrix.
				if (i == nRow || j == nCol)
				{
					continue;
				}

				///Store the value in TrimmedMatrix.linear and increment the TrimedMatrixCounter.
				TrimedMatrix.m_linear[TrimedMatrixCounter++] = m_matrix[i][j];

				///if i have all 4 values set on the TrimedMatrixCounter then break the for.
				if (TrimedMatrixCounter == 9)
				{
					break;
				}
			}

		///Return the Cofactor which is the Determinant of the 3x3 Matrix 
		/// [ A11 A12 A13 ]
		/// [ A21 A22 A23 ]
		/// [ A31 A32 A33 ]
		return TrimedMatrix.Determinant();
	}

	void Matrix4::Transpose() noexcept
	{
		///Transpose the Matrix
		Matrix4 R = *this;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m_matrix[j][i] = R.m_matrix[i][j];
	}

	void Matrix4::Identity() noexcept
	{
		///Make it an identity Matrix
		for (int i = 0; i < 16; i++)
		{
			m_linear[i] = 0;
		}

		m_matrix[0][0] = 1.0f;
		m_matrix[1][1] = 1.0f;
		m_matrix[2][2] = 1.0f;
		m_matrix[3][3] = 1.0f;
	}

	Matrix4 Matrix4::GetTransposed() const noexcept
	{
		///Returns a this matrix transposed
		Matrix4 Temp = *this;

		Temp.Transpose();
		return Temp;
	}

	float Matrix4::Determinant() const
	{
		float Det = 0;
		float sign = -1;

		for (uint32 i = 0; i < 4; i++)
		{
			Det += CoFactor3x3(i, 0)*Math::Pow((-1), gsl::narrow_cast<float>(i))*m_matrix[i][0];
			sign *= -1;
		}

		return Det;
	}

	Matrix4 Matrix4::GetAdjoint() const
	{
		Matrix4 Adj(eInit::None);

		for (uint32 i = 0; i < 4; i++)
			for (uint32 j = 0; j < 4; j++)
			{
				Adj.m_matrix[i][j] = Math::Pow((-1), gsl::narrow_cast<float>(i + j))*CoFactor3x3(i, j);
			}

		Adj.Transpose();

		return Adj;
	}

	Matrix4 Matrix4::GetInverse() const
	{
		const float Det = Determinant();

		if (Det == 0)
		{
			return Matrix4(eInit::Zero);
		}

		Matrix4 Temp = GetAdjoint();
		Temp *= 1.0f / Det;

		return Temp;
	}

	Quaternion Matrix4::GetAsQuaternion()
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

	Matrix4 Matrix4::Translate3D(float xDelta, float yDelta, float zDelta)
	{
		///Create an Identity Matrix
		Matrix4 Temp(eInit::Unit);

		///Make it a Translation Matrix
		/// [ 1 0 0 xDelta ]
		/// [ 0 1 0 yDelta ]
		/// [ 0 0 1	zDelta ]
		/// [ 0 0 0    1   ]
		Temp.m_matrix[3][0] = xDelta;
		Temp.m_matrix[3][1] = yDelta;
		Temp.m_matrix[3][2] = zDelta;

		///Return Temp
		return Temp;
	}

	Matrix4 Matrix4::Scale3D(float xScale, float yScale, float zScale)
	{
		///Create an Identity Matrix
		Matrix4 Temp(eInit::Unit);


		///Make it a Scale Matrix
		/// [ xScale  0      0    0 ]
		/// [   0   yScale   0    0 ]
		/// [   0     0    zScale 0 ]
		/// [   0     0      0    1 ]
		Temp.m_matrix[0][0] = xScale;
		Temp.m_matrix[1][1] = yScale;
		Temp.m_matrix[2][2] = zScale;

		///Return Temp
		return Temp;
	}

	Matrix4 Matrix4::Rotate3D(Degree xRotation, Degree yRotation, Degree zRotation)
	{
		///Create a 3D rotation Matrix using the rczAXIS given
		Matrix4 x(eInit::Unit), y(eInit::Unit), z(eInit::Unit);

		const float xtheta = xRotation.ValueRadian().Value();
		const float ytheta = yRotation.ValueRadian().Value();
		const float ztheta = zRotation.ValueRadian().Value();

		int LeftHand = -1;

		if (xRotation != 0)
		{
			x.m_matrix[1][1] = x.m_matrix[2][2] = Math::Cos(xtheta);
			x.m_matrix[1][2] = Math::Sin(xtheta)*LeftHand;
			x.m_matrix[2][1] = -Math::Sin(xtheta)*LeftHand;
		}

		if (yRotation != 0)
		{
			y.m_matrix[0][0] = y.m_matrix[2][2] = Math::Cos(ytheta);
			y.m_matrix[0][2] = -Math::Sin(ytheta)*LeftHand;
			y.m_matrix[2][0] = Math::Sin(ytheta)*LeftHand;
		}

		if (zRotation != 0)
		{
			z.m_matrix[0][0] = z.m_matrix[1][1] = Math::Cos(ztheta);
			z.m_matrix[0][1] = Math::Sin(ztheta)*LeftHand;
			z.m_matrix[1][0] = -Math::Sin(ztheta)*LeftHand;
		}

		return z * y*x;
	}

	Matrix4 Matrix4::OrthoProjectedSpace(float width, float height, float ZNear, float ZFar)
	{
		Matrix4 Temp(eInit::Unit);

		float fRange = 1.0f / (ZFar - ZNear);

		Temp.m_matrix[0][0] = 2.0f / width;
		Temp.m_matrix[1][1] = 2.0f / height;
		Temp.m_matrix[2][2] = fRange;
		Temp.m_matrix[2][3] = -fRange * ZNear;

		return Temp;
	}

	Matrix4 Matrix4::PerpsProjectedSpace(float FovY, float AspectRatio, float ZNear, float ZFar)
	{
		Matrix4 Temp(eInit::Zero);

		float fRange = ZFar / (ZFar - ZNear);
		float SinFov = Math::Sin(FovY*0.5f);
		float CosFov = Math::Cos(FovY*0.5f);
		float Height = CosFov / SinFov;

		Temp.m_matrix[1][1] = CosFov / SinFov;
		Temp.m_matrix[0][0] = Height / AspectRatio;
		Temp.m_matrix[2][2] = fRange;
		Temp.m_matrix[2][3] = 1.0f;
		Temp.m_matrix[3][2] = -(fRange)*ZNear;

		// CosFov / SinFov,0,0,0
		// 0,Height / AspectHByW,0,0
		// 0,0,fRange,1.0f
		// 0,0,-fRange * NearZ,0.0f

		return Temp;
	}

	Matrix4 Matrix4::LookAtMatrix(Vector3 eye, Vector3 up, Vector3 target) noexcept
	{
		///Get the zAxis of the camera by constructing the vector from the eye to the target
		Vector3 zAxisCamera = target - eye;
		zAxisCamera.Normalize();
		///Get the right vector of the camera by doing a cross product of the up vector and the zAxis
		Vector3 xAxisCamera = up ^ zAxisCamera;
		xAxisCamera.Normalize();
		///Get the yAAxis by doing a cross product of the xAxis and the xAxis
		const Vector3 yAxisCamera = zAxisCamera ^ xAxisCamera;


		Matrix4 LookAt = {
			Vector4(xAxisCamera.m_x, yAxisCamera.m_x, zAxisCamera.m_x, 0),
			Vector4(xAxisCamera.m_y, yAxisCamera.m_y, zAxisCamera.m_y, 0),
			Vector4(xAxisCamera.m_z, yAxisCamera.m_z, zAxisCamera.m_z, 0),
			Vector4(-(xAxisCamera | eye), -(yAxisCamera | eye), -(zAxisCamera | eye), 1)
		};

		return LookAt;
	}

	Matrix4 Matrix4::LookAtMatrixZ(Vector3 eye, Vector3 up, Vector3 target) noexcept
	{
		///Get the zAxis of the camera by constructing the vector from the eye to the target
		Vector3 zAxisCamera = target - eye;
		zAxisCamera.Normalize();
		///Get the right vector of the camera by doing a cross product of the up vector and the zAxis
		Vector3 xAxisCamera = up ^ zAxisCamera;
		xAxisCamera.Normalize();
		///Get the yAAxis by doing a cross product of the xAxis and the xAxis
		const Vector3 yAxisCamera = zAxisCamera ^ xAxisCamera;


		const Matrix4 LookAt = {
			Vector4(yAxisCamera.m_x, zAxisCamera.m_x, xAxisCamera.m_x, 0),
			Vector4(yAxisCamera.m_y, zAxisCamera.m_y, xAxisCamera.m_y, 0),
			Vector4(yAxisCamera.m_z, zAxisCamera.m_z, xAxisCamera.m_z, 0),
			Vector4(-(xAxisCamera | eye), -(yAxisCamera | eye), -(zAxisCamera | eye), 1)
		};

		return LookAt;
	}

	Matrix4 Matrix4::LookAtMatrixY(Vector3 eye, Vector3 up, Vector3 target) noexcept
	{
		///Get the zAxis of the camera by constructing the vector from the eye to the target
		Vector3 zAxisCamera = target - eye;
		zAxisCamera.Normalize();
		///Get the right vector of the camera by doing a cross product of the up vector and the zAxis
		Vector3 xAxisCamera = up ^ zAxisCamera;
		xAxisCamera.Normalize();
		///Get the yAAxis by doing a cross product of the xAxis and the xAxis
		const Vector3 yAxisCamera = zAxisCamera ^ xAxisCamera;


		const Matrix4 LookAt = {
			Vector4(zAxisCamera.m_x, xAxisCamera.m_x, yAxisCamera.m_x, 0),
			Vector4(zAxisCamera.m_y, xAxisCamera.m_y, yAxisCamera.m_y, 0),
			Vector4(zAxisCamera.m_z, xAxisCamera.m_z, yAxisCamera.m_z, 0),
			Vector4(-(xAxisCamera | eye), -(yAxisCamera | eye), -(zAxisCamera | eye), 1)
		};

		return LookAt;
	}

	Matrix4 Matrix4::operator*(const float& scalar) const noexcept
	{
		Matrix4 Temp = *this;

#pragma omp for
		for (int32 i = 0; i < 16; ++i)
			Temp.m_linear[i] *= scalar;

		return Temp;
	}

	Matrix4 Matrix4::operator* (const Matrix4& M) const noexcept
	{
		const Matrix4 Temp = M.GetTransposed();
		const Matrix4 This = *this;
		Matrix4 TempReturn(eInit::None);

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				TempReturn.m_matrix[i][j] = This.m_rows[i] | Temp.m_rows[j];
			}

		return TempReturn;
	}

	Vector4 Matrix4::operator* (const Vector4& V) const noexcept
	{
		const Matrix4 Temp = *this;
		Vector4 NewVector(eInit::None);

		NewVector.m_x = Temp.m_rows[0].m_x * V.m_x + Temp.m_rows[1].m_x * V.m_y + Temp.m_rows[2].m_x * V.m_z + Temp.m_rows[3].m_x * V.m_w;
		NewVector.m_y = Temp.m_rows[0].m_y * V.m_x + Temp.m_rows[1].m_y * V.m_y + Temp.m_rows[2].m_y * V.m_z + Temp.m_rows[3].m_y * V.m_w;
		NewVector.m_z = Temp.m_rows[0].m_z * V.m_x + Temp.m_rows[1].m_z * V.m_y + Temp.m_rows[2].m_z * V.m_z + Temp.m_rows[3].m_z * V.m_w;
		NewVector.m_w = Temp.m_rows[0].m_w * V.m_x + Temp.m_rows[1].m_w * V.m_y + Temp.m_rows[2].m_w * V.m_z + Temp.m_rows[3].m_w * V.m_w;

		return NewVector;
	}

	Vector3 Matrix4::operator*(const Vector3 & V) const noexcept
	{
		const Matrix4 Temp = *this;
		Vector3 NewVector(eInit::None);

		NewVector.m_x = Temp.m_rows[0].m_x * V.m_x + Temp.m_rows[1].m_x * V.m_y + Temp.m_rows[2].m_x * V.m_z;
		NewVector.m_y = Temp.m_rows[0].m_y * V.m_x + Temp.m_rows[1].m_y * V.m_y + Temp.m_rows[2].m_y * V.m_z;
		NewVector.m_z = Temp.m_rows[0].m_z * V.m_x + Temp.m_rows[1].m_z * V.m_y + Temp.m_rows[2].m_z * V.m_z;

		return NewVector;
	}

	void Matrix4::operator*=(float scalar) noexcept
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				m_matrix[i][j] *= scalar;
			}
	}

	void Matrix4::operator*=(Matrix4 M) noexcept
	{
		*this = *this*M;
	}
}