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
		m_Planes[eFrustum::fBottom].ConstructFromPoints(FarPoints[2], NearPoints[2], NearPoints[3]);

		m_Planes[eFrustum::fLeft].ConstructFromPoints(FarPoints[0], NearPoints[0], NearPoints[3]);
		m_Planes[eFrustum::fRight].ConstructFromPoints(FarPoints[2], NearPoints[1], NearPoints[1]);

		m_Planes[eFrustum::fNear].ConstructFromPoints(NearPoints[0], NearPoints[1], NearPoints[2]);
		m_Planes[eFrustum::fFar].ConstructFromPoints(FarPoints[2], FarPoints[1], FarPoints[1]);

	}
}
