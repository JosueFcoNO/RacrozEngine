#pragma once

namespace rczEngine
{
	class RZ_EXP Scene
	{
	public:
		///Calls Destroy.
		~Scene()
		{
			Destroy();
		}; 

		void InitScene(const char* name);
		void ClearScene();
		void Update(float deltaTime);
		void Destroy();

		WeakGameObjectPtr CreateActor(const char* name = "GameObj", GameObject* parent = NULL, Vector3 position = { 0,0,0 }, Vector3 orientation = { 0,0,0 }, Vector3 scale = { 1,1,1 });
		StrCmpPtr CreateComponent(eCOMPONENT_ID type, GameObjectID owner);
		StrCmpPtr CreateComponent(eCOMPONENT_ID type, StrGameObjectPtr owner);

		WeakGameObjectPtr FindActor(GameObjectID id);
		WeakGameObjectPtr FindActor(const char* name);

		void AddRootNodeChild(StrGameObjectPtr node);
		void RemoveGameObject(GameObjectID actorId);

		void AddActor(StrGameObjectPtr node);
		Vector<WeakPtr<GameObject>> FindActorsWithComponent(eCOMPONENT_ID cmp);

		Gfx::ConstantBuffer m_WorldMatrix;

		Gfx::GfxCore* m_gfx;
		ResVault* m_res;
		LightManager* m_light;

		StrGameObjectMap m_SceneActorMap;
		StrGameObjectPtr m_RootNode;

	private:
		String m_Name;
		
		Vector<StrPtr<GameObject>> m_GameObjectsToDelete;
	};

}