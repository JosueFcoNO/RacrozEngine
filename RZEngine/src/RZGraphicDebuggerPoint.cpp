#include <RZEnginePCH.h>

namespace rczEngine
{
	void DebuggerPoint::SetPoint(const Vector3 & pos, float radius) noexcept
	{
		m_VertexBuffer.Destroy();

		auto vLeftX = pos;
		auto vRightX = pos;
		auto vLeftZ = pos;
		auto vRightZ = pos;
		auto vUpY = pos;
		auto vDownY = pos;

		vLeftX.m_x -= radius / 2.0f;
		vRightX.m_x += radius / 2.0f;

		vLeftZ.m_z -= radius / 2.0f;
		vRightZ.m_z += radius / 2.0f;

		vUpY.m_y += radius / 2.0f;
		vDownY.m_y -= radius / 2.0f;

		m_VertexBuffer.AddVertex(vLeftX);
		m_VertexBuffer.AddVertex(vRightX);
		m_VertexBuffer.AddVertex(vLeftZ);
		m_VertexBuffer.AddVertex(vRightZ);
		m_VertexBuffer.AddVertex(vUpY);
		m_VertexBuffer.AddVertex(vDownY);

		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, Gfx::GfxCore::Pointer());
	}
}