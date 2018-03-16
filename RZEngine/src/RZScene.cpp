#include <RZEnginePCH.h>

namespace rczEngine
{
	void Scene::InitScene()
	{
		m_gfx = Gfx::GfxCore::Pointer();
		m_res = ResVault::Pointer();

		m_RootNode = std::make_shared<GameObject>(0, "RootNode");
		m_RootNode->Init();

		m_RootNode->Translate(Vector3(0.0f, 0.0f, 0.0f));
		m_RootNode->Scale(Vector3(1.0f, 1.0f, 1.0f));
		m_RootNode->Rotate(Vector3(0.0f, 0.0f, 0.0f));

		m_WorldMatrix.CreateConstantBuffer(sizeof(Matrix4), Gfx::eBUFFER_USAGE::USAGE_DEFAULT, m_gfx);
	}

	void Scene::Update(float deltaTime)
	{
		m_RootNode->Update(this, deltaTime);
	}

	void Scene::Destroy()
	{
		m_SceneActorMap.clear();
		m_WorldMatrix.Destroy();

		m_RootNode->Destroy();
	}

	
	WeakGameObjectPtr Scene::CreateActor(char * name, GameObject * parent, Vector3 position, Vector3 orientation, Vector3 scale)
	{
		auto ptr = m_ActFactory.CreateActor(name, position, orientation, scale);
		AddActor(ptr);
		if (parent)
		{
			parent->AddChild(ptr);
		}
		else
		{
			m_RootNode->AddChild(ptr);
		}

		return ptr;
	}

	WeakGameObjectPtr Scene::FindActor(GameObjectID id)
	{
		StrGameObjectMap::iterator it;
		it = m_SceneActorMap.find(id);

		if (it == m_SceneActorMap.end())
		{
			return WeakGameObjectPtr();
		}

		return it->second;
	}

	WeakGameObjectPtr Scene::FindActor(char * name)
	{
		auto it = m_SceneActorMap.begin();
		for (; it != m_SceneActorMap.end(); ++it)
		{
			if (it->second->GetName() == name)
			{
				return it->second;
			}
		}

		if (it == m_SceneActorMap.end())
		{
			return (WeakGameObjectPtr());
		}
	}

	void Scene::AddRootNodeChild(StrGameObjectPtr node)
	{
		m_RootNode->AddChild(node);
	}

	void Scene::RemoveChild(GameObjectID actorId)
	{
		if (actorId == INVALID_ID)
		{
			return;
		}

		auto DeadMan = FindActor(actorId);
		m_SceneActorMap.erase(actorId);
		m_RootNode->RemoveChild(actorId);
	}

	void Scene::AddActor(StrGameObjectPtr node)
	{
		m_SceneActorMap[node->GetID()] = node;
	}

	Vector<WeakPtr<GameObject>> Scene::FindActorsWithComponent(eCOMPONENT_ID cmp)
	{
		Vector<WeakPtr<GameObject>> TempVector;

		for (auto it = m_SceneActorMap.begin(); it != m_SceneActorMap.end(); ++it)
		{
			if (it->second->GetComponent(cmp).lock())
			{
				TempVector.push_back(it->second);
			}
		}

		return TempVector;
	}
}