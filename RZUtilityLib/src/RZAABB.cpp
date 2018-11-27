#pragma once
#include <RZUtilityPCH.h>
#include "RZAABB.h"

namespace rczEngine
{
	void AABB::SetAABB(Vector3 minV, Vector3 maxV)
	{
		m_PointMax = maxV;
		m_PointMin = minV;
	}

	void AABB::SetAABB(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax)
	{
		m_PointMax.Set(xMax, yMax, zMax);
		m_PointMin.Set(xMin, yMin, zMin);
	}

	void AABB::SetAABBc(Vector3 center, Vector3 size)
	{
		m_PointMin = center - (size / 2);
		m_PointMax = center + (size / 2);
	}

	void AABB::SetAABBc(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
	{
		Vector3 center(xCenter, yCenter, zCenter);
		Vector3 size(xSize, ySize, zSize);

		m_PointMin = center - (size / 2);
		m_PointMax = center + (size / 2);
	}

	void AABB::AddPoint(Vector3 point)
	{
			if (point.m_x < m_PointMin.m_x) m_PointMin.m_x = point.m_x;
			if (point.m_x > m_PointMax.m_x) m_PointMax.m_x = point.m_x;
			if (point.m_y < m_PointMin.m_y) m_PointMin.m_y = point.m_y;
			if (point.m_y > m_PointMax.m_y) m_PointMax.m_y = point.m_y;
			if (point.m_z < m_PointMin.m_z) m_PointMin.m_z = point.m_z;
			if (point.m_z > m_PointMax.m_z) m_PointMax.m_z = point.m_z;
	}

	void AABB::Clear()
	{
		m_PointMin.Set(0, 0, 0);
		m_PointMax.Set(0.000001f, 0.000001f, 0.000001f);
	}

	void AABB::TransformAndRecalculateAABB(Matrix3 newMatrix)
	{
		Vector3 size = GetSize();
		Vector3 points[8];
		points[0] = m_PointMin;
		points[1] = m_PointMin + Vector3(size.m_x,0,0);
		points[2] = m_PointMin + Vector3(0,0, size.m_z);
		points[3] = m_PointMin + Vector3(size.m_x, 0, size.m_z);

		points[4] = m_PointMax;
		points[5] = m_PointMax + Vector3(-size.m_x, 0, 0);
		points[6] = m_PointMax + Vector3(0, 0, -size.m_z);
		points[7] = m_PointMax + Vector3(-size.m_x, 0, -size.m_z);

		for (int32 i = 0; i < 8; ++i)
		{
			points[i] = newMatrix*points[i];
		}

		Clear();
		
		for (int32 i = 0; i < 8; ++i)
		{
			AddPoint(points[i]);
		}
	}

	bool AABB::CheckPointCollision(const Vector3 & point)
	{
		return (point >= m_PointMin && point <= m_PointMax);
	}

	Vector3 AABB::GetCenter()
	{
		return (m_PointMax + m_PointMin) / 2;
	}

	Vector3 AABB::GetSize()
	{
		return m_PointMax - m_PointMin;
	}

	Vector3 AABB::GetRadius()
	{
		return GetSize() / 2;
	}

	void AABB::SetCenter(Vector3 center)
	{
		m_PointMax += center;
		m_PointMin += center;
	}
}