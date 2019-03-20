#pragma once
#include <RZUtilityPCH.h>
#include "RZPlane.h"

namespace rczEngine
{
	///////////////////////////////////////////////
	////////Plane
	///////////////////////////////////////////////

	Plane::Plane() noexcept
	{
		Normal.Set(0, 1, 0);
		D = 0;
	}

	Plane::Plane(float x, float y, float z, float w) noexcept
	{
		Normal.Set(x, y, z);
		D = w;
	}

	Plane::Plane(const Vector3& normal, float d) noexcept
	{ 
		Normal = normal;
		D = d;
	}

	Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3) noexcept : D(0)
	{
		ConstructFromPoints(p1, p2, p3);
	}

	Plane::Plane(const Vector3 & p1, const Vector3 & normal) noexcept
	{
		ConstructFromPointNormal(p1, normal);
	}


	void Plane::ConstructFromPointNormal(const Vector3 &Pt, const Vector3 &normal) noexcept
	{
		Normal = normal;
		D = -(Pt | normal);
	}

	void Plane::ConstructFromPointVectors(const Vector3 &Pt, const Vector3 &V1, const Vector3 &V2) noexcept
	{
		const Vector3 normal = V1^V2;
		ConstructFromPointNormal(Pt, normal.GetNormalized());
	}

	void Plane::ConstructFromPoints(const Vector3 &V0, const Vector3 &V1, const Vector3 &V2) noexcept
	{
		const Vector3 normal = ((V1 - V0) ^ (V2 - V0)).GetNormalized();
		return ConstructFromPointNormal(V0, normal);
	}

	void Plane::Construct(float a, float b, float c, float d) noexcept
	{
		Normal.Set(a, b, c);
		Normal.Normalize();
		D = d;
	}
}