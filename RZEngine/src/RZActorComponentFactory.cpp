#include <RZEnginePCH.h>

namespace rczEngine
{
	StrActorPtr ActorComponentFactory::CreateActor(char* name, Vector3 position, Vector3 orientation, Vector3 scale)
	{
		///Create a strong pointer to the new actor
		StrActorPtr Temp(new SNode( GetNewActorId(), name ) );

		///Init the new Actor
		Temp->Init();

		Temp->Translate(position);
		Temp->Scale(scale);
		Temp->Rotate(orientation);

		///Init his components

		///Return the temp
		return Temp;
	};
}