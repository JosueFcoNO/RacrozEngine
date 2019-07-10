#pragma once

namespace rczEngine
{
	class RZ_EXP DebuggerPoint : public GraphicDebuggerElement
	{
	public:
		void SetPoint(const Vector3& pos, float radius) noexcept;
		virtual uint8 GetElementType() { return DEBUG_POINT; };

	private:
		Vector3 m_Position;
	};
}