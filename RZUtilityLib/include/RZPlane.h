#pragma once

namespace rczEngine
{
	class RZ_EXP Plane
	{
	public:
		Plane() noexcept;
		Plane(float x, float y, float z, float w) noexcept;
		Plane(const Vector3& Normal, float w) noexcept;
		Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3) noexcept;
		Plane(const Vector3& p1, const Vector3& normal) noexcept;

		FORCEINLINE void ConstructFromPointNormal(const Vector3 &Pt, const Vector3 &normal) noexcept;
		FORCEINLINE void ConstructFromPointVectors(const Vector3 &Pt, const Vector3 &V1, const Vector3 &V2) noexcept;
		FORCEINLINE void ConstructFromPoints(const Vector3 &V0, const Vector3 &V1, const Vector3 &V2) noexcept;
		FORCEINLINE void Construct(float a, float b, float c, float d) noexcept;

		FORCEINLINE float SignedDistance(const Vector3 &Pt) const noexcept { return ((Pt|Normal) + D); }
		FORCEINLINE float UnsignedDistance(const Vector3 &Pt) const noexcept { return abs((Pt | Normal) + D); }

		static Vector3 ThreePlaneIntersect(const Plane one, const Plane two, const Plane three) noexcept;
		static bool PlaneLineIntersection(const Plane& p, const Vector3& linePoint1, const Vector3& linePoint2, Vector3& out_pointOfIntersection) noexcept;

		Vector3 Normal;
		float D;
		Vector3 Point;
	};
}