#pragma once

namespace rczEngine
{
	class RZ_EXP Plane
	{
	public:
		Plane();
		Plane(float x, float y, float z, float w);
		Plane(const Vector3& Normal, float w);
		Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);
		Plane(const Vector3& p1, const Vector3& normal);

		void ConstructFromPointNormal(const Vector3 &Pt, const Vector3 &normal);
		void ConstructFromPointVectors(const Vector3 &Pt, const Vector3 &V1, const Vector3 &V2);
		void ConstructFromPoints(const Vector3 &V0, const Vector3 &V1, const Vector3 &V2);

		float SignedDistance(const Vector3 &Pt) const
		{
			return ((Pt|Normal) + D);
		}

		float UnsignedDistance(const Vector3 &Pt) const
		{
			return abs((Pt | Normal) + D);
		}

		Vector3 Normal;
		float D;
	};
}