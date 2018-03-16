#pragma once 
#include <RZUtilityPCH.h>
#include "RZSphere.h"

namespace rczEngine
{
	Sphere::Sphere() : m_Position(INIT_ZERO)
	{
		m_Radius = 1.0f;
	}

	Sphere::Sphere(float x, float y, float z, float Radius) : m_Position(INIT_NONE)
	{
		m_Radius = Radius;
		m_Position.m_x = x;
		m_Position.m_y = y;
		m_Position.m_z = z;
	}

	void Sphere::Set(float x, float y, float z, float Radius)
	{
		m_Radius = Radius;
		m_Position.m_x = x;
		m_Position.m_y = y;
		m_Position.m_z = z;
	}

	bool Sphere::CheckSphereCollision(Sphere sph)
	{
		Vector3 Temp = sph.m_Position - m_Position;
		if (Temp.Magnitude() <= m_Radius + sph.m_Radius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Vector3 Sphere::GetPosition()
	{
		return m_Position;
	}

	float Sphere::GetRadius()
	{
		return m_Radius;
	}
}