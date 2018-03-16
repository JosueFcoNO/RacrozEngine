#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT DebuggerLine : public GraphicDebuggerElement
	{
	public:
		void Render(Gfx::GfxCore* graphicsapi_instance);
		virtual uint8 GetElementType() { return DEBUG_LINE; };
	};
}