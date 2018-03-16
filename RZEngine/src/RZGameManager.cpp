#include <RZEnginePCH.h>

namespace rczEngine
{
	void GameManager::UpdateActor(float deltaTime)
	{
	}

	void GameManager::AddActor(StrGameObjectPtr actor)
	{
		Pair<GameObjectID, StrGameObjectPtr> P(actor->GetID(), actor);
		m_ActorMap.insert(P);
	}

	void GameManager::RemoveActor(GameObjectID actorID)
	{
		m_ActorMap.erase(actorID);
	}

	StrGameObjectPtr GameManager::GetActor(GameObjectID actorID)
	{
		return m_ActorMap[actorID].lock();
	}
};