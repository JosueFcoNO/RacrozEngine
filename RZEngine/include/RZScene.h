#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Scene
	{
	public:
		///Calls Destroy.
		~Scene()
		{
			Destroy();
		}; 

		void InitScene();
		void Update(float deltaTime);
		void Destroy();

		WeakGameObjectPtr CreateActor(char* name = "GameObj", GameObject* parent = NULL, Vector3 position = { 0,0,0 }, Vector3 orientation = { 0,0,0 }, Vector3 scale = { 1,1,1 });

		WeakGameObjectPtr FindActor(GameObjectID id);
		WeakGameObjectPtr FindActor(char* name);

		void AddRootNodeChild(StrGameObjectPtr node);
		void RemoveChild(GameObjectID actorId);

		void AddActor(StrGameObjectPtr node);
		Vector<WeakPtr<GameObject>> FindActorsWithComponent(eCOMPONENT_ID cmp);

		Gfx::ConstantBuffer m_WorldMatrix;

		Gfx::GfxCore* m_gfx;
		ResVault* m_res;
		LightManager* m_light;

		StrGameObjectMap m_SceneActorMap;
		StrGameObjectPtr m_RootNode;

	private:
		ActorComponentFactory m_ActFactory;
	};

}