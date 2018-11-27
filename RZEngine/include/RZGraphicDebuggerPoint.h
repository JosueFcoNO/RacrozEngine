#pragma once

namespace rczEngine
{
	class RZ_EXP DebuggerPoint : public GraphicDebuggerElement
	{
	public:
		DebuggerPoint() : m_Position(INIT_ZERO)
		{
		};

		void Render(Gfx::GfxCore* gfx);
		virtual uint8 GetElementType() { return DEBUG_POINT; };
		float m_Radius;
		Vector3 m_Position;
		ResourceHandle handle;
	};
}