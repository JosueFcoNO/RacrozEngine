#pragma once

namespace rczEngine
{
	class RZ_EXP DebuggerCube : public GraphicDebuggerElement
	{
	public:
		void Render(Gfx::GfxCore* graphicsapi_instance);
		virtual uint8 GetElementType() { return DEBUG_CUBE; };
	};
};