#include <RZEnginePCH.h>

namespace rczEngine
{
	void SkinnedModelRenderer::Update(float deltaTime)
	{
		if (m_Model == 0)
		{
			return;
		}

		auto Res = ResVault::Pointer();

		WeakPtr<SkinnedModel> Model = Res->GetResource<SkinnedModel>(m_Model);
		WeakPtr<Animation> Anim;

		if (Model.lock()->m_SetAnimation != INVALID_RESOURCE)
		{
			Anim = Res->GetResource<Animation>(Model.lock()->m_SetAnimation);

			m_CurrentTime += deltaTime*Anim.lock()->m_TicksPerSecond;

			m_CurrentTime = Math::fMod(m_CurrentTime, Anim.lock()->m_Duration);
		}

		Model.lock()->m_MeshSkeleton.UpdateSkeleton(Anim, m_CurrentTime);
	}
}
