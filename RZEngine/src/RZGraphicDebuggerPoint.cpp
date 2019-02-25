#include <RZEnginePCH.h>

namespace rczEngine
{
	void DebuggerPoint::Render(Gfx::GfxCore * graphicsapi_instance)
	{
		ResVault::Pointer()->GetResource<Model>(0).lock()->DrawModel(NULL);
	}
}