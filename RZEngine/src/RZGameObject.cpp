#include <RZEnginePCH.h>

namespace rczEngine
{
	void GameObject::Init()
	{
		for (auto It = m_Components.begin(); It != m_Components.end(); It++)
		{
			(*It).second->Init();
		}
	}

	void GameObject::Update(Scene* scene, float deltaTime)
	{
		UpdateWorldMatrix();

		///Go through all the actor's components and update them
		for (auto It = m_Components.begin(); It != m_Components.end(); It++)
		{
			(*It).second->Update(deltaTime);
		}

		for (int i = 0; i < m_ChildrenVector.size(); ++i)
		{
			m_ChildrenVector[i].lock()->Update(scene, deltaTime);
		}
	}

	void GameObject::Destroy()
	{

		m_ToDestroy = true;
	}

	void GameObject::PreRender(Scene * scene)
	{
		scene->m_WorldMatrix.UpdateConstantBuffer(&m_ToWorld.GetTransposed(), scene->m_gfx);
		scene->m_WorldMatrix.SetBufferInVS(2, scene->m_gfx);
		scene->m_WorldMatrix.SetBufferInDS(2, scene->m_gfx);
	}

	void GameObject::Render(Scene * scene, ComponentType cmpType, MATERIAL_TYPE mat)
	{

		if (cmpType == CMP_MODEL_RENDERER)
		{
			auto component = GetComponent<ModelRenderer>(CMP_MODEL_RENDERER);

			if (component.lock())
			{
				component.lock()->Render(scene->m_gfx, scene->m_res, scene, mat);
			}
		}
		else if (cmpType == CMP_SKINNED_MODEL_RENDERER)
		{
			auto component = GetComponent<SkinnedModelRenderer>(CMP_SKINNED_MODEL_RENDERER);

			if (component.lock())
			{
				component.lock()->RenderSkinned(scene->m_gfx, scene->m_res, scene, mat);
			}
		}
		else if (cmpType == CMP_TERRAIN_RENDERER)
		{
			auto component = GetComponent<TerrainRenderer>(CMP_TERRAIN_RENDERER);

			if (component.lock())
			{
				component.lock()->Render(scene->m_gfx, scene->m_res, scene);
			}
		}
	}

	void GameObject::UpdateWorldMatrix()
	{
		if (m_ParentNode.lock())
		{
			m_ToWorld = GetLocalMatrix()*m_ParentNode.lock()->m_ToWorld;
		}
		else
		{
			m_ToWorld = GetLocalMatrix(false);
		}
	}

	WeakCmpPtr GameObject::AddComponent(eCOMPONENT_ID cmp)
	{
		StrCmpPtr P;

		switch (cmp)
		{
		case CMP_MODEL_RENDERER:
			P = std::make_shared<ModelRenderer>();
			break;

		case CMP_SKINNED_MODEL_RENDERER:
			P = std::make_shared<SkinnedModelRenderer>();
			break;

		case CMP_MOVE:
			P = std::make_shared<SimpleMove>();
			break;
		}

		m_Components[cmp] = P;
		P->SetOwner(SceneManager::Pointer()->GetActiveScene()->FindActor(m_GameObjectID).lock());

		P->Init();

		return P;
	}

#ifndef RZ_EDITOR
	void GameObject::RenderComponents()
	{

		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			it->second->RenderComponent();
		}

	}
#endif

	Matrix4 GameObject::GetLocalMatrix(bool HasParent)
	{
		if (m_DirtyLocalMatrix)
			m_ToLocal = HasParent ?
			Matrix4::Scale3D(m_Scale.m_x, m_Scale.m_y, m_Scale.m_z)*
			Matrix4::Rotate3D(m_Orientation.m_x, m_Orientation.m_y, m_Orientation.m_z)*
			Matrix4::Translate3D(m_Position.m_x, m_Position.m_y, m_Position.m_z)
			:
			Matrix4::Scale3D(m_Scale.m_x, m_Scale.m_y, m_Scale.m_z)*
			Matrix4::Translate3D(m_Position.m_x, m_Position.m_y, m_Position.m_z)*
			Matrix4::Rotate3D(m_Orientation.m_x, m_Orientation.m_y, m_Orientation.m_z);

		m_DirtyLocalMatrix = false;
		return m_ToLocal;
	}

}