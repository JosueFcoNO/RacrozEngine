#include <RZEnginePCH.h>

namespace rczEngine
{
	StrGameObjectPtr ActorComponentFactory::CreateActor(char* name, Vector3 position, Vector3 orientation, Vector3 scale)
	{
		///Create a strong pointer to the new actor
		StrGameObjectPtr Temp = std::make_shared<GameObject>(GetNewActorId(), name);

		///Init the new Actor
		Temp->Init();

		Temp->Translate(position);
		Temp->Scale(scale);
		Temp->Rotate(orientation);

		///Return the temp, the temp pointer created here stops existing.
		return Temp;
	};
}