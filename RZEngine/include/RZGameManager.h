#pragma once

namespace rczEngine
{
	class RZ_EXP GameManager
	{
	public:
		void UpdateActor(float deltaTime);
		void AddActor(StrGameObjectPtr actor);
		void RemoveActor(GameObjectID actorID);
		StrGameObjectPtr GetActor(GameObjectID actorID);

		WeakGameObjectMap m_ActorMap;
	};
};