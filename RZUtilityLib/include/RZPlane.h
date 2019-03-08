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

		void ConstructFromPointNormal(const Vector3 &Pt, const Vector3 &normal) noexcept;
		void ConstructFromPointVectors(const Vector3 &Pt, const Vector3 &V1, const Vector3 &V2) noexcept;
		void ConstructFromPoints(const Vector3 &V0, const Vector3 &V1, const Vector3 &V2) noexcept;
		void Construct(float a, float b, float c, float d) noexcept;

		float SignedDistance(const Vector3 &Pt) const noexcept
		{
			return ((Pt|Normal) + D);
		}

		float UnsignedDistance(const Vector3 &Pt) const noexcept
		{
			return abs((Pt | Normal) + D);
		}

		static Vector3 ThreePlaneIntersect(const Plane one, const Plane two, const Plane three)
		{
			auto twoThreeNCross = (two.Normal^three.Normal);

			const Vector3 a = twoThreeNCross *-one.D;
			const Vector3 b = (three.Normal^one.Normal)*-two.D;
			const Vector3 c = (one.Normal^two.Normal)*-three.D;
			const float denom = one.Normal | twoThreeNCross;

			return (a+b+c) / denom;
		}

		Vector3 Normal;
		float D;
	};
}