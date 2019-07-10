#include <RZEnginePCH.h>

namespace rczEngine
{
	void DebuggerCube::SetCube(const Vector3 & center, const Vector3 & scale)
	{
		m_VertexBuffer.Destroy();

		const auto xScale = scale.m_x / 2.0f;
		const auto yScale = scale.m_y / 2.0f;
		const auto zScale = scale.m_z / 2.0f;

		const auto topBackLeft = Vector3{ center.m_x - xScale, center.m_y + yScale, center.m_z + zScale };
		const auto topBackRight = Vector3{ center.m_x + xScale, center.m_y + yScale, center.m_z + zScale };
		const auto topFrontLeft = Vector3{ center.m_x - xScale, center.m_y + yScale, center.m_z - zScale };
		const auto topFrontRight = Vector3{ center.m_x + xScale, center.m_y + yScale, center.m_z - zScale };

		const auto bottomBackLeft = Vector3{ center.m_x - xScale, center.m_y - yScale, center.m_z + zScale };
		const auto bottomBackRight = Vector3{ center.m_x + xScale, center.m_y - yScale, center.m_z + zScale };
		const auto bottomFrontLeft = Vector3{ center.m_x - xScale, center.m_y - yScale, center.m_z - zScale };
		const auto bottomFrontRight = Vector3{ center.m_x + xScale, center.m_y - yScale, center.m_z - zScale };

		m_VertexBuffer.AddVertex(topBackLeft);
		m_VertexBuffer.AddVertex(topBackRight);
		m_VertexBuffer.AddVertex(topFrontRight);
		m_VertexBuffer.AddVertex(topFrontLeft);

		m_VertexBuffer.AddVertex(bottomFrontRight);
		m_VertexBuffer.AddVertex(bottomFrontLeft);
		m_VertexBuffer.AddVertex(bottomBackLeft);
		m_VertexBuffer.AddVertex(bottomBackRight);

		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DYNAMIC, false, Gfx::GfxCore::Pointer());
	}
};