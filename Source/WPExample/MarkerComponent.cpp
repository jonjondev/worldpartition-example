// Fill out your copyright notice in the Description page of Project Settings.

#include "MarkerComponent.h"

#if WITH_EDITORONLY_DATA
const FName UMarkerComponent::WithMarkerTag = FName("WithMarker");
#endif

UMarkerComponent::UMarkerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMarkerComponent::PostInitProperties()
{
	Super::PostInitProperties();
#if WITH_EDITORONLY_DATA
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		AActor* Actor = GetOwner();
		if (IsValid(Actor) && !Actor->HasAnyFlags(RF_ClassDefaultObject))
		{
			if (!Actor->Tags.Contains(WithMarkerTag))
			{
				Actor->Tags.AddUnique(WithMarkerTag);
				Actor->MarkPackageDirty();
			}
		}
	}
#endif
}

#if WITH_EDITOR
void UMarkerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Also check for the actor's
	// Tell the subsystem an update is needed?
}
#endif
