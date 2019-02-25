#pragma once
#include <RZUtilityPCH.h>
#include "RZVector4D.h"

namespace rczEngine
{

	///////////////////////////////////////////////
	////////VECTOR 4D
	///////////////////////////////////////////////

	Vector4::Vector4(eInit init) noexcept
	{
		
#pragma omp simd
		for (int32 i = 0; i < 4; ++i)
		{
			m_elements[i] = 0.0f;
		}

		if (init == eInit::Unit)
		{
			m_x = 1;
		}
	}

	void Vector4::Set(const float& x, const float& y, const float& z, const float& w) noexcept
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	void Vector4::Scale(const float& scale) noexcept
	{
#pragma omp simd
		for (int32 i = 0; i < 4; ++i)
		{
			gsl::at(m_elements, i) *= scale;
		}
	}

	Vector4& Vector4::operator+=(const Vector4& v) noexcept
	{
#pragma omp simd
		for (int32 i = 0; i < 4; ++i)
		{
			gsl::at(m_elements, i) += gsl::at(v.m_elements, i);
		}

		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& v) noexcept
	{
#pragma omp simd
		for (int32 i = 0; i < 4; ++i)
		{
			gsl::at(m_elements, i) -= gsl::at(v.m_elements, i);
		}

		return *this;
	}

	Vector4& Vector4::operator*=(const Vector4& v) noexcept
	{
#pragma omp simd
		for (int32 i = 0; i < 4; ++i)
		{
			gsl::at(m_elements, i) *= gsl::at(v.m_elements, i);
		}

		return *this;
	}

	Vector4& Vector4::operator*=(float f) noexcept
	{
#pragma omp simd
		for (int32 i = 0; i < 4; ++i)
		{
			gsl::at(m_elements, i) *= f;
		}

		return *this;
	}

	Vector4& Vector4::operator/=(const Vector4& v) noexcept
	{
#pragma omp simd
		for (int32 i = 0; i < 4; ++i)
		{
			gsl::at(m_elements, i) /= gsl::at(v.m_elements, i);
		}

		return *this;
	}

	Vector4& Vector4::operator/=(float f) noexcept
	{
#pragma omp simd
		for (int32 i = 0; i < 4; ++i)
		{
			gsl::at(m_elements, i) /= f;
		}

		return *this;
	}

	void Vector4::Normalize() noexcept
	{
		const float Mag = Magnitude();

#pragma omp simd
		for (int32 i = 0; i < 4; ++i)
		{
			gsl::at(m_elements, i) /= Mag;
		}
	}

	Vector4 Vector4::GetNormalized() const noexcept
	{
		Vector4 temp(*this);
		temp.Normalize();
		return temp;
	}

	float Vector4::Magnitude() const noexcept
	{
		return Math::Sqrt((Math::Square(m_x) + Math::Square(m_y) + Math::Square(m_z) + Math::Square(m_w))); 
	}

}