#include <RZEnginePCH.h>

namespace rczEngine
{
	void DebuggerLine::Render(Gfx::GfxCore * graphicsapi_instance)
	{
		graphicsapi_instance->DrawIndexed(2, m_IndexOffset, 0);
	}
};