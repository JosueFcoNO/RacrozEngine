#include <RZEnginePCH.h>

namespace rczEngine
{
	void DebuggerCube::Render(Gfx::GfxCore * graphicsapi_instance)
	{
		graphicsapi_instance->DrawIndexed(24, m_IndexOffset, 0);
	}
};