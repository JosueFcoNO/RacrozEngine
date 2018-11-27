#include <RZEnginePCH.h>

namespace rczEngine
{
	void DebuggerPoint::Render(Gfx::GfxCore * graphicsapi_instance)
	{
		ResVault::Pointer()->GetResource<Model>(NULL).lock()->DrawModel(graphicsapi_instance, ResVault::Pointer(), NULL);
	}
}