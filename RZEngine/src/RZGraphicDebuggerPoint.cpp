#include <RZEnginePCH.h>

namespace rczEngine
{
	void DebuggerPoint::Render(Gfx::GfxCore * graphicsapi_instance)
	{
		ResVault::Pointer()->GetResource<Model>(ResVault::Pointer()->m_ModelCube).lock()->DrawModel(NULL);
	}
}