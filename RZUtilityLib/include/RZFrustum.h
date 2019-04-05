#pragma once

namespace rczEngine
{
	enum eFrustum
	{
		fNear,
		fFar,
		fLeft,
		fRight,
		fTop,
		fBottom,
		fMax_Sides
	};

	class RZ_EXP Frustum
	{
		public:
			void CalculateFrustum(const Camera& camera);
			
			bool TestPoint(const Vector3 & point) const noexcept;
			bool TestLineSegment(const Vector3 & p0, const Vector3 & p1) const noexcept;
			bool TestAABB(const AABB& aabb) const noexcept;
			
			Vector4 NearPoints[4];
			Vector4 FarPoints[4];

		private:
			Plane m_Planes[eFrustum::fMax_Sides];


	};
}