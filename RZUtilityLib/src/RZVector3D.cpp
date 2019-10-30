#pragma once
#include <RZUtilityPCH.h>

namespace rczEngine
{
	Vector3::Vector3(eInit init) noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif
		
		if (init == eInit::Unit)
		{
			m_x = 1;
			m_y = 0;
			m_z = 0;
		}
		else
		{
			m_x = 0;
			m_y = 0;
			m_z = 0;
		}
	}

	///Sets the vector
	void Vector3::Set(const float& x, const float& y, const float& z) noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif

		m_x = x;
		m_y = y;
		m_z = z;
	}

	void Vector3::Set(const Vector3 & v) noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif

		*this = v;
	}

	///Scales the vector by scale
	void Vector3::Scale(const float& Scale) noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif

#pragma omp simd
		for (int32 i = 0; i < 3; ++i)
		{
			m_elements[i] *= Scale;
		}
	}

	Vector3::Vector3(const Vector4 & v4) noexcept :
		m_x(v4.m_x),
		m_y(v4.m_y),
		m_z(v4.m_z) {}

	///Returns the distance of the vector3: vhead-vtail.
	float Vector3::Distance(const Vector3 & vhead, const Vector3 & vtail) noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif

		return (vhead - vtail).Magnitude();
	}

	///Returns the distance of the vector3: vhead-vtail.
	float Vector3::SqrDistance(const Vector3 & vhead, const Vector3 & vtail) noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif

		return (vhead - vtail).SqrMagnitude();
	}

	///returns the angle of the x and y components of the vector
	float Vector3::Anglexz(const Vector3 & v, bool getAsDegree) noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif

		Vector3 temp(v);
		temp.Normalize();

		if (getAsDegree)
		{
			return Math::RadiansToDegrees(Math::aTan2(temp.m_z, temp.m_x)+180.0f);
		}
		else
		{
			return Math::aTan2(temp.m_z, temp.m_x) + PI2;
		}
	}

	uint32 Vector3::Hash(const Vector3 & v, int mult) noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif

		const auto x = gsl::narrow_cast<int32>(v.m_x * mult);
		const auto y = gsl::narrow_cast<int32>(v.m_y * mult);
		const auto z = gsl::narrow_cast<int32>(v.m_z * mult);

		return (x * 73856093) ^ (y * 83492791) ^ (z * 19349663);
	}

	Vector3& Vector3::operator+=(const Vector3& v) noexcept
	{
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;

		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& v) noexcept
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
		m_z -= v.m_z;

		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& v) noexcept
	{
		m_x *= v.m_x;
		m_y *= v.m_y;
		m_z *= v.m_z;

		return *this;
	}

	Vector3& Vector3::operator*=(const float& f) noexcept
	{
		m_x *= f;
		m_y *= f;
		m_z *= f;

		return *this;
	}

	Vector3& Vector3::operator/=(const Vector3& v) noexcept
	{
		m_x /= v.m_x;
		m_y /= v.m_y;
		m_z /= v.m_z;

		return *this;
	}

	Vector3& Vector3::operator/=(const float& f) noexcept
	{
		m_x /= f;
		m_y /= f;
		m_z /= f;

		return *this;
	}

	Vector3 Vector3::operator^(const Vector3& v) const noexcept
	{
		const float x = m_y*v.m_z - m_z*v.m_y;
		const float y = m_z*v.m_x - m_x*v.m_z;
		const float z = m_x*v.m_y - m_y*v.m_x;

		return Vector3(x, y, z);
	}

	void Vector3::Normalize() noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif
		const float Mag = Magnitude();

#pragma omp simd
		for (int32 i = 0; i < 3; ++i)
		{
			m_elements[i] /= Mag;
		}
	}

	Vector3 Vector3::GetNormalized() const noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif

		Vector3 Temp(*this);
		Temp.Normalize();
		return Temp;
	}

	float Vector3::Magnitude() const noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif
		float add = 0.0f;

#pragma omp simd
		for (int i = 0; i < 3; ++i)
		{
			add += Math::Square(m_elements[i]);
		}

		return Math::Sqrt(add);
	}

	float Vector3::SqrMagnitude() const noexcept
	{
#ifdef RZ_PROFILING
		ProfilerObj autoProfiler(RZ__FUNCTION__, PROFILE_EVENTS::PROF_GAME);
#endif
		float add = 0.0f;

#pragma omp simd
		for (int i = 0; i < 3; ++i)
		{
			add += Math::Square(m_elements[i]);
		}

		return add;
	}

	Vector3 Vector3::operator+(const Vector3 & v) const noexcept
	{
		return Vector3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
	}

	Vector3 Vector3::operator-(const Vector3 & v) const noexcept
	{
		return Vector3(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
	}

	Vector3 Vector3::operator*(const Vector3 & v) const noexcept
	{
		return Vector3(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z);
	}

	Vector3 Vector3::operator/(const Vector3 & v) const noexcept
	{
		return Vector3(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z);
	}

	Vector3 Vector3::operator*(float f) const noexcept
	{
		return Vector3(m_x * f, m_y * f, m_z * f);
	}

	Vector3 Vector3::operator/(float f) const noexcept
	{
		return Vector3(m_x / f, m_y/ f, m_z / f);
	}
}