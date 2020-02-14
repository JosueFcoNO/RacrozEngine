#include <RZEnginePCH.h>

namespace rczEngine
{
	ActorComponentFactory*& ActorComponentFactory::_Instance()
	{
		static ActorComponentFactory* instance = nullptr;
		return instance;
	}

	void ActorComponentFactory::Start()
	{
		(_Instance()) = new ActorComponentFactory;
	}

	ActorComponentFactory* ActorComponentFactory::Pointer()
	{
		return _Instance();
	}

	void ActorComponentFactory::ShutDown()
	{
		delete _Instance();
	}

	StrGameObjectPtr ActorComponentFactory::CreateActor(const String&& sceneName, const String&& name, Vector3 position, Vector3 orientation, Vector3 scale)
	{
		///Create a strong pointer to the new actor
		StrGameObjectPtr Temp = std::make_shared<GameObject>(GetNewActorId(), name);

		///Init the new Actor
		Temp->Init(std::move(sceneName));

		Temp->SetPosition(position);
		Temp->SetScale(scale);
		Temp->SetOrientation(orientation);

		///Return the temp, the temp pointer created here stops existing.
		return Temp;
	}

	StrCmpPtr ActorComponentFactory::CreateComponent(eComponentID component, WeakGameObjPtr owner)
	{
		StrCmpPtr P;

		switch (component)
		{
		case CMP_MODEL_RENDERER:
			P = std::make_shared<ModelRenderer>();
			break;

		case CMP_SKINNED_MODEL_RENDERER:
			P = std::make_shared<SkinnedModelRenderer>();
			break;
		case CMP_BONE:
			P = std::make_shared<BoneComponent>();
			break;
		case CMP_LIGHT:
			P = std::make_shared<LightComponent>();
			break;

		case CMP_MOVE:
			P = std::make_shared<SimpleMove>();
			break;

		case CMP_SPACE_MANAGER:
			P = std::make_shared<SpaceComponent>();
			break;

		case CMP_CAMERA:
			P = std::make_shared<CameraCmp>();
			break;

		case CMP_CAMERA_WALK:
			P = std::make_shared<CameraWalk>();
			break;

		case CMP_CAMERA_GHOST:
			P = std::make_shared<CameraGhost>();
			break;
		}

		P->m_ID = ++m_LastCmpID;
		P->SetOwner(owner.lock());

		owner.lock()->AddComponent(component, P);

		return P;
	}
	
}