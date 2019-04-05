#include <RZUtilityPCH.h>

namespace rczEngine
{
	void Frustum::CalculateFrustum(const Camera& camera)
	{
		const auto fovRad = Math::DegreesToRad(camera.GetFov());

		const auto HNear = 2 * Math::Tan(fovRad / 2.0f) * camera.GetNearClip();
		const auto WNear = HNear * camera.GetAspectRatio();

		const auto HFar = 2 * Math::Tan(fovRad / 2.0f) * camera.GetFarClip();
		const auto WFar = HFar * camera.GetAspectRatio();

		const auto nearCenter = camera.GetPosition() + camera.GetViewDir() * camera.GetNearClip();

		NearPoints[0] = nearCenter + (camera.GetUp() * HNear / 2.0f) - (camera.GetRight() * WNear / 2.0f);
		NearPoints[1] = nearCenter + (camera.GetUp() * HNear / 2.0f) + (camera.GetRight() * WNear / 2.0f);
		NearPoints[2] = nearCenter - (camera.GetUp() * HNear / 2.0f) + (camera.GetRight() * WNear / 2.0f);
		NearPoints[3] = nearCenter - (camera.GetUp() * HNear / 2.0f) - (camera.GetRight() * WNear / 2.0f);

		const auto farCenter = camera.GetPosition() + camera.GetViewDir() * camera.GetFarClip();

		FarPoints[0] = farCenter + (camera.GetUp() * HFar / 2.0f) - (camera.GetRight() * WFar / 2.0f);
		FarPoints[1] = farCenter + (camera.GetUp() * HFar / 2.0f) + (camera.GetRight() * WFar / 2.0f);
		FarPoints[2] = farCenter - (camera.GetUp() * HFar / 2.0f) + (camera.GetRight() * WFar / 2.0f);
		FarPoints[3] = farCenter - (camera.GetUp() * HFar / 2.0f) - (camera.GetRight() * WFar / 2.0f);

		m_Planes[eFrustum::fTop].ConstructFromPoints(FarPoints[0], FarPoints[1], NearPoints[1]);
		m_Planes[eFrustum::fBottom].ConstructFromPoints(NearPoints[2], FarPoints[2], FarPoints[3]);

		m_Planes[eFrustum::fLeft].ConstructFromPoints(FarPoints[0], NearPoints[0], NearPoints[3]);
		m_Planes[eFrustum::fRight].ConstructFromPoints(NearPoints[2], NearPoints[1], FarPoints[1]);

		m_Planes[eFrustum::fNear].ConstructFromPoints(NearPoints[0], NearPoints[1], NearPoints[2]);
		m_Planes[eFrustum::fFar].ConstructFromPoints(FarPoints[2], FarPoints[1], FarPoints[0]);
	}

	bool Frustum::TestPoint(const Vector3 & point) const noexcept
	{
		if (m_Planes[0].SignedDistance(point) > 0.0f)
		if (m_Planes[2].SignedDistance(point) > 0.0f)
		if (m_Planes[3].SignedDistance(point) > 0.0f)
		if (m_Planes[4].SignedDistance(point) > 0.0f)  
		if (m_Planes[5].SignedDistance(point) > 0.0f)
		{
			return true;
		}

		return false;
	}

	bool Frustum::TestLineSegment(const Vector3& p0, const Vector3& p1) const noexcept
	{

		auto ints = 0;

#pragma omp for
		for (auto& plane : m_Planes)
		{
			if (Plane::PlaneLineIntersection(plane, p0, p1)) ints++;
		}

		return (ints > 1);
	}

	bool Frustum::TestAABB(const AABB & aabb) const noexcept
	{
		Vector<Vector3> points = aabb.GetCorners();

		//if (TestPoint(aabb.GetCenter())) return true;
		//
		//for (auto pt : points)
		//{
		//	if (TestPoint(pt)) return true;
		//}

		if (TestLineSegment(points[0], points[1])) return true;
		if (TestLineSegment(points[1], points[2])) return true;
		if (TestLineSegment(points[2], points[3])) return true;
		if (TestLineSegment(points[0], points[3])) return true;

		if (TestLineSegment(points[4], points[5])) return true;
		if (TestLineSegment(points[5], points[6])) return true;
		if (TestLineSegment(points[6], points[7])) return true;
		if (TestLineSegment(points[4], points[7])) return true;

		//if (TestLineSegment(points[0], points[4])) return true;
		//if (TestLineSegment(points[1], points[5])) return true;
		//if (TestLineSegment(points[2], points[6])) return true;
		//if (TestLineSegment(points[3], points[7])) return true;

		return false;
	}
}
