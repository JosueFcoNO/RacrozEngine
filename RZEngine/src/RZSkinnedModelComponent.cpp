#include <RZEnginePCH.h>

namespace rczEngine
{
	void SkinnedModelRenderer::Update(float deltaTime)
	{
		auto Res = ResVault::Pointer();


		WeakPtr<SkinnedModel> Model = Res->GetResource<SkinnedModel>(m_Model);
		WeakPtr<Animation> Anim;

		if (Model.lock()->m_SetAnimation != INVALID_RESOURCE)
		{
			Anim = Res->GetResource<Animation>(Model.lock()->m_SetAnimation);

			m_CurrentTime += deltaTime*Anim.lock()->m_TicksPerSecond;
			if (m_CurrentTime > Anim.lock()->m_Duration)
			{
				m_CurrentTime = 0.0f;
			}
		}

		Model.lock()->m_MeshSkeleton.UpdateSkeleton(Anim, m_CurrentTime);
	}
}
