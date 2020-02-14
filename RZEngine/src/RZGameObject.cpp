#include <RZEnginePCH.h>

namespace rczEngine
{
	void GameObject::Init(const String&& owningScene)
	{
		m_OwningScene = SceneManager::Pointer()->GetScene(owningScene);

		for (auto It = m_Components.begin(); It != m_Components.end(); It++)
		{
			(*It).second->Init();
		}
	}

	void GameObject::Update(float deltaTime)
	{
		if (m_ToDestroy) return;

		UpdateWorldMatrix();

		///Go through all the actor's components and update them
#pragma omp parallel for
		for (auto It = m_Components.begin(); It != m_Components.end(); It++)
		{
			(*It).second->Update(deltaTime);
		}

#pragma omp parallel for
		for (int i = 0; i < m_ChildrenVector.size(); ++i)
		{
			m_ChildrenVector[i].lock()->Update(deltaTime);
		}
	}

	void GameObject::Destroy(bool DestroyParentRef)
	{
		//Set myself to be destroyed
		m_ToDestroy = true;

		//Clear my components
		m_Components.clear();

		//Destroy reference in parent
		//if (DestroyParentRef)
		//	m_ParentNode.lock()->RemoveChild(m_GameObjectID);

		//Remove myself from the scene
		if (m_GameObjectID != 0)
			SceneManager::Pointer()->GetActiveScene()->RemoveGameObject(m_GameObjectID);

		//Destroy all my children
		for (int i = 0; i < m_ChildrenVector.size(); ++i)
		{
			if (!m_ChildrenVector[i].expired())
				m_ChildrenVector[i].lock()->Destroy();
		}

		m_ChildrenIDs.clear();
		m_ChildrenVector.clear();
	}

	void GameObject::PreRender()
	{
		m_ToWorld[0].Transpose();

		auto scene = m_OwningScene.lock();

		scene->m_WorldMatrix.UpdateConstantBuffer(m_ToWorld, scene->m_gfx);
		scene->m_WorldMatrix.SetBufferInVS(2, scene->m_gfx);
		scene->m_WorldMatrix.SetBufferInDS(2, scene->m_gfx);

		m_ToWorld[0].Transpose();
	}

	void GameObject::Render(ComponentType cmpType, int renderHash)
	{	
		auto scene = m_OwningScene.lock();
		auto gfx = scene->m_gfx;
		auto res = scene->m_res;

		if (cmpType == CMP_MODEL_RENDERER)
		{
			auto component = GetComponent<ModelRenderer>(CMP_MODEL_RENDERER).lock();

			if (component)
			{
				component->Render(gfx, res, renderHash);
			}
		}
		else if (cmpType == CMP_SKINNED_MODEL_RENDERER)
		{
			auto component = GetComponent<SkinnedModelRenderer>(CMP_SKINNED_MODEL_RENDERER).lock();

			if (component)
			{
				//component->RenderSkinned(gfx, res, mat);
			}
		}
		else if (cmpType == CMP_TERRAIN_RENDERER)
		{
			auto component = GetComponent<TerrainRenderer>(CMP_TERRAIN_RENDERER).lock();

			if (component)
			{
				component->Render(gfx, res);
			}
		}
		else if (cmpType == CMP_SPACE_MANAGER)
		{
			auto component = GetComponent<SpaceComponent>(CMP_SPACE_MANAGER).lock();

			if (component)
			{
				component->Render(gfx, res);
			}
		}
	}

	void GameObject::UpdateWorldMatrix()
	{
		m_ToWorld[1] = m_ToWorld[0].GetTransposed();

		if (m_ParentNode.lock())
		{
			m_ToWorld[0] = GetLocalMatrix()*m_ParentNode.lock()->m_ToWorld[0];
		}
		else
		{
			m_ToWorld[0] = GetLocalMatrix();
		}
	}

	WeakCmpPtr GameObject::AddComponent(eComponentID cmp, StrCmpPtr ptr)
	{
		m_Components[cmp] = ptr;
		ptr->Init();

		return ptr;
	}

#ifndef RZ_EDITOR


	Vector<WeakCmpPtr> GameObject::GetRenderComponents()
	{
		Vector<WeakCmpPtr> renderComps;

		for (auto cmp : m_Components)
		{
			auto ptr = cmp.second;

			if (ptr->IsRendereable())
			{
				renderComps.push_back(cmp.second);
			}
		}

		return renderComps;
	}

	bool GameObject::HasRenderComponents()
	{
		for (auto cmp : m_Components)
		{
			auto& ptr = *cmp.second;

			if (ptr.IsRendereable())
			{
				return true;
			}
		}

		return false;
	}

	void GameObject::RenderComponents()
	{

		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			it->second->RenderComponent();
		}

	}
#endif

	void GameObject::Serialize()
	{
		auto ser = Serializer::Pointer();
		ser->SetNextObjectSerial(SERIAL_ACTOR);

		//ID
		ser->WriteData(&m_GameObjectID, sizeof(m_GameObjectID));

		//Name
		char name[32];
		m_Name.shrink_to_fit();
		for (int i = 0; i < m_Name.size(); ++i)
		{
			name[i] = m_Name[i];
		}
		ser->WriteData(name, 32);

		//Position
		ser->WriteData(&m_Position.m_x, sizeof(float));
		ser->WriteData(&m_Position.m_y, sizeof(float));
		ser->WriteData(&m_Position.m_z, sizeof(float));

		//Rotation
		ser->WriteData(&m_Orientation.m_x, sizeof(float));
		ser->WriteData(&m_Orientation.m_y, sizeof(float));
		ser->WriteData(&m_Orientation.m_z, sizeof(float));

		//Scaling
		ser->WriteData(&m_Scale.m_x, sizeof(float));
		ser->WriteData(&m_Scale.m_y, sizeof(float));
		ser->WriteData(&m_Scale.m_z, sizeof(float));

		//ParentID
		int id = m_ParentID;
		ser->WriteData(&id, sizeof(id));

		//Child number
		id = (int)m_ChildrenIDs.size();
		ser->WriteData(&id, sizeof(id));

		//Child Ids
		for (int i = 0; i < m_ChildrenIDs.size(); ++i)
		{
			id = m_ChildrenIDs[i];
			ser->WriteData(&id, sizeof(id));
		}

		//Has Components
		ser->SetNextObjectSerial(SERIAL_COMPONENT);

		//Number Of Components
		id = (int)m_Components.size();
		ser->WriteData(&id, sizeof(id));

		if (id)
			for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
			{
				it->second->Serialize();
			}
	}

	void GameObject::DeSerialize()
	{
		auto ser = Serializer::Pointer();

		//ID
		ser->ReadData(&m_GameObjectID, sizeof(m_GameObjectID));

		//Name
		char name[32];
		ser->ReadData(name, 32);
		m_Name = name;

		//Position
		ser->ReadData(&m_Position.m_x, sizeof(float));
		ser->ReadData(&m_Position.m_y, sizeof(float));
		ser->ReadData(&m_Position.m_z, sizeof(float));

		//Rotation
		ser->ReadData(&m_Orientation.m_x, sizeof(float));
		ser->ReadData(&m_Orientation.m_y, sizeof(float));
		ser->ReadData(&m_Orientation.m_z, sizeof(float));

		//Scaling
		ser->ReadData(&m_Scale.m_x, sizeof(float));
		ser->ReadData(&m_Scale.m_y, sizeof(float));
		ser->ReadData(&m_Scale.m_z, sizeof(float));

		//ParentID
		ser->ReadData(&m_ParentID, sizeof(m_ParentID));

		//Child number
		int size;
		ser->ReadData(&size, sizeof(size));

		//Child Ids
		for (int i = 0; i < size; ++i)
		{
			int id;
			ser->ReadData(&id, sizeof(id));

			m_ChildrenIDs.push_back(id);
		}

		//Has Components
		ser->GetNextObjectSerial();

		int nComponents;

		//Number Of Components
		ser->ReadData(&nComponents, sizeof(int));

		auto scenePtr = SceneManager::Pointer()->GetActiveScene();

		for (int32 i = 0; i < nComponents; ++i)
		{
			eComponentID cmpType = (eComponentID)(ser->GetNextObjectSerial() - SERIAL_COMPONENT_OFFSET);
			scenePtr->CreateComponent(cmpType, shared_from_this())->DeSerialize();
		}
	}

	void GameObject::GetGraphPointers(int idOffset)
	{
		auto sceneMng = SceneManager::Pointer();
		auto scene = sceneMng->GetActiveScene();

		m_GameObjectID += idOffset;

		if (m_ParentID == 0)
		{
			scene->m_RootNode->AddChild(shared_from_this());
		}
		else
		{
			m_ParentID += idOffset;
			m_ParentNode = scene->FindActor(m_ParentID);
		}

		for (int i = 0; i < m_ChildrenIDs.size(); ++i)
		{
			m_ChildrenIDs[i] += idOffset;
			m_ChildrenVector.push_back(scene->FindActor(m_ChildrenIDs[i]));
		}
	}

	Matrix4 GameObject::GetLocalMatrix()
	{
		if (m_DirtyLocalMatrix)
			m_ToLocal =
			Matrix4::Scale3D(m_Scale.m_x, m_Scale.m_y, m_Scale.m_z)*
			Matrix4::Rotate3D(m_Orientation.m_x, m_Orientation.m_y, m_Orientation.m_z)*
			Matrix4::Translate3D(m_Position.m_x, m_Position.m_y, m_Position.m_z);

		m_DirtyLocalMatrix = false;
		return m_ToLocal;
	}

}