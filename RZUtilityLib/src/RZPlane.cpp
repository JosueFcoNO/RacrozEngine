#pragma once
#include <RZUtilityPCH.h>
#include "RZPlane.h"

namespace rczEngine
{
	///////////////////////////////////////////////
	////////Plane
	///////////////////////////////////////////////

	Plane::Plane()
	{
		Normal.Set(0, 1, 0);
		D = 0;
	}

	Plane::Plane(float x, float y, float z, float w)
	{
		Normal.Set(x, y, z);
		D = w;
	}

	Plane::Plane(const Vector3& normal, float d)
	{ 
		Normal = normal;
		D = d;
	}

	Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
	{
		ConstructFromPoints(p1, p2, p3);
	}


	void Plane::ConstructFromPointNormal(const Vector3 &Pt, const Vector3 &normal)
	{
		Vector3 NormalizedNormal = normal;
		NormalizedNormal.Normalize();

		Normal = NormalizedNormal;
		D = -(Pt | NormalizedNormal);
	}

	void Plane::ConstructFromPointVectors(const Vector3 &Pt, const Vector3 &V1, const Vector3 &V2)
	{
		Vector3 normal = V1^V2;
		ConstructFromPointNormal(Pt, normal);
	}

	void Plane::ConstructFromPoints(const Vector3 &V0, const Vector3 &V1, const Vector3 &V2)
	{
		Vector3 normal = ((V1 - V0) ^ (V2 - V0)).GetNormalized();
		return ConstructFromPointNormal(V0, normal);
	}
}