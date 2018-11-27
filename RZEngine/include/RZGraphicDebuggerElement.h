#pragma once

namespace rczEngine
{
	enum eDEBUG_ELEMENT
	{
		DEBUG_LINE,
		DEBUG_POINT,
		DEBUG_CUBE
	};

	class RZ_EXP GraphicDebuggerElement
	{
	public:
		GraphicDebuggerElement() : m_Color(INIT_ZERO)
		{
		};
		~GraphicDebuggerElement()
		{
		};
		virtual void Render(Gfx::GfxCore* graphicsapi_instance) = 0;
		virtual uint8 GetElementType() = 0;

		uint32 m_IndexOffset;
		Vector4 m_Color;
	};
}