#include <RZEnginePCH.h>

namespace rczEngine
{
	void DebuggerPoint::Render(Gfx::GfxCore * graphicsapi_instance)
	{
		ResVault* ptr = ResVault::Pointer();
		m_PointModel->DrawModel(graphicsapi_instance, ptr, NULL);
	}
}