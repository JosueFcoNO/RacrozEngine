#include <RZUtilityPCH.h>

namespace rczEngine
{
	void Frustum::CalculateFrustum(const Matrix4 & viewMat, const Matrix4 & projMatrix, const Matrix4 & camTransform)
	{
		Matrix4 mat = projMatrix * viewMat;

		m_Planes[eFrustum::fTop].Construct(mat.m_matrix[0][3] - mat.m_matrix[0][1], mat.m_matrix[1][3] - mat.m_matrix[1][1], mat.m_matrix[2][3] - mat.m_matrix[2][1], -mat.m_matrix[3][3] + mat.m_matrix[3][1]);
		m_Planes[eFrustum::fBottom].Construct(mat.m_matrix[0][3] + mat.m_matrix[0][1], mat.m_matrix[1][3] + mat.m_matrix[1][1], mat.m_matrix[2][3] + mat.m_matrix[2][1], -mat.m_matrix[3][3] - mat.m_matrix[3][1]);
		m_Planes[eFrustum::fLeft].Construct(mat.m_matrix[0][3] + mat.m_matrix[0][0], mat.m_matrix[1][3] + mat.m_matrix[1][0], mat.m_matrix[2][3] + mat.m_matrix[2][0], -mat.m_matrix[3][3] - mat.m_matrix[3][0]);
		m_Planes[eFrustum::fRight].Construct(mat.m_matrix[0][3] - mat.m_matrix[0][0], mat.m_matrix[1][3] - mat.m_matrix[1][0], mat.m_matrix[2][3] - mat.m_matrix[2][0], -mat.m_matrix[3][3] + mat.m_matrix[3][0]);
		m_Planes[eFrustum::fNear].Construct(mat.m_matrix[0][3] + mat.m_matrix[0][2], mat.m_matrix[1][3] + mat.m_matrix[1][2], mat.m_matrix[2][3] + mat.m_matrix[2][2], -mat.m_matrix[3][3] - mat.m_matrix[3][2]);
		m_Planes[eFrustum::fFar].Construct(mat.m_matrix[0][3] - mat.m_matrix[0][2], mat.m_matrix[1][3] - mat.m_matrix[1][2], mat.m_matrix[2][3] - mat.m_matrix[2][2], -mat.m_matrix[3][3] + mat.m_matrix[3][2]);
			
		NearPoints[0] = Plane::ThreePlaneIntersect(m_Planes[fTop], m_Planes[fNear], m_Planes[fLeft]);
		NearPoints[1] = Plane::ThreePlaneIntersect(m_Planes[fTop], m_Planes[fNear], m_Planes[fRight]);
		NearPoints[2] = Plane::ThreePlaneIntersect(m_Planes[fBottom], m_Planes[fNear], m_Planes[fLeft]);
		NearPoints[3] = Plane::ThreePlaneIntersect(m_Planes[fBottom], m_Planes[fNear], m_Planes[fRight]);

		FarPoints[0] = Plane::ThreePlaneIntersect(m_Planes[fTop], m_Planes[fFar], m_Planes[fLeft]);
		FarPoints[1] = Plane::ThreePlaneIntersect(m_Planes[fTop], m_Planes[fFar], m_Planes[fRight]);
		FarPoints[2] = Plane::ThreePlaneIntersect(m_Planes[fBottom], m_Planes[fFar], m_Planes[fLeft]);
		//FarPoints[3] = Plane::ThreePlaneIntersect(m_Planes[fBottom], m_Planes[fFar], m_Planes[fRight]);

		FarPoints[3] = Plane::ThreePlaneIntersect(Plane({ 0,-1,0 }, { 0, 1, 0 }), Plane({ -1,0,0 }, { 1, 0, 0 }), Plane({ 0,0,-1 }, { 0, 0, 1 }));
	}
}
