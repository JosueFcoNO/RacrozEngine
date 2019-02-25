#pragma once 
#include <RZUtilityPCH.h>
#include "RZSphere.h"

namespace rczEngine
{
	void Sphere::Set(float x, float y, float z, float Radius) noexcept
	{
		m_Radius = Radius;
		m_Position.m_x = x;
		m_Position.m_y = y;
		m_Position.m_z = z;
	}

	bool Sphere::CheckSphereCollision(Sphere sph) const noexcept
	{
		return (Vector3::Distance(sph.m_Position, m_Position) <= m_Radius + sph.m_Radius);
	}

	Vector3 Sphere::GetPosition() const noexcept
	{
		return m_Position;
	}

	float Sphere::GetRadius() const noexcept
	{
		return m_Radius;
	}
}