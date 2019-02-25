#pragma once
#include <RZUtilityPCH.h>
#include "RZAABB.h"

namespace rczEngine
{
	void AABB::SetAABB(Vector3 minV, Vector3 maxV) noexcept
	{
		m_PointMax = maxV;
		m_PointMin = minV;
	}

	void AABB::SetAABB(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax) noexcept
	{
		m_PointMax.Set(xMax, yMax, zMax);
		m_PointMin.Set(xMin, yMin, zMin);
	}

	void AABB::SetAABBc(Vector3 center, Vector3 size) noexcept
	{
		m_PointMin = center - (size / 2);
		m_PointMax = center + (size / 2);
	}

	void AABB::SetAABBc(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize) noexcept
	{
		const Vector3 center(xCenter, yCenter, zCenter);
		const Vector3 size(xSize, ySize, zSize);

		m_PointMin = center - (size / 2);
		m_PointMax = center + (size / 2);
	}

	void AABB::AddPoint(Vector3 point) noexcept
	{
			if (point.m_x < m_PointMin.m_x) m_PointMin.m_x = point.m_x;
			if (point.m_x > m_PointMax.m_x) m_PointMax.m_x = point.m_x;
			if (point.m_y < m_PointMin.m_y) m_PointMin.m_y = point.m_y;
			if (point.m_y > m_PointMax.m_y) m_PointMax.m_y = point.m_y;
			if (point.m_z < m_PointMin.m_z) m_PointMin.m_z = point.m_z;
			if (point.m_z > m_PointMax.m_z) m_PointMax.m_z = point.m_z;
	}

	void AABB::Clear() noexcept
	{
		m_PointMin.Set(0, 0, 0);
		m_PointMax.Set(0.000001f, 0.000001f, 0.000001f);
	}

	void AABB::TransformAndRecalculateAABB(Matrix3 newMatrix)
	{
		const Vector3 size = GetSize();
		Vector3 points[8] =
		{
			m_PointMin,
			m_PointMin + Vector3(size.m_x, 0, 0),
			m_PointMin + Vector3(0, 0, size.m_z),
			m_PointMin + Vector3(size.m_x, 0, size.m_z),

			m_PointMax,
			m_PointMax + Vector3(-size.m_x, 0, 0),
			m_PointMax + Vector3(0, 0, -size.m_z),
			m_PointMax + Vector3(-size.m_x, 0, -size.m_z)
		};

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
}