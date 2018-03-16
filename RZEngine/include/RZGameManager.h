#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT GameManager
	{
	public:
		void UpdateActor(float deltaTime);
		void AddActor(StrGameObjectPtr actor);
		void RemoveActor(GameObjectID actorID);
		StrGameObjectPtr GetActor(GameObjectID actorID);

		WeakGameObjectMap m_ActorMap;
	};
};