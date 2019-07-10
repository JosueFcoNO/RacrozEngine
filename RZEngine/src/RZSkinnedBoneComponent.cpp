#include <RZEnginePCH.h>

namespace rczEngine
{
	void BoneComponent::Update(float deltaTime)
	{
		if (!once)
		{
			//GraphicDebugger::Pointer()->AddPoint(std::to_string(m_ObjID), m_Owner.lock()->GetAccumulatedPosition(), 0.1f, Color(0, 1, 0));
			once = true;
		}
	}

	void BoneComponent::Render(Gfx::GfxCore * gfx, ResVault * res, Scene * scene, MATERIAL_TYPE matType)
	{

	}
}