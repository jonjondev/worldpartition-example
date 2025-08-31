// Fill out your copyright notice in the Description page of Project Settings.

#include "MarkerSubsystem.h"

#include "MarkerComponent.h"
#include "WorldPartition/WorldPartition.h"
#include "WorldPartition/WorldPartitionHelpers.h"

AMarkerCollection::AMarkerCollection()
{
	PrimaryActorTick.bCanEverTick = false;
#if WITH_EDITORONLY_DATA
	bIsSpatiallyLoaded = false;
#endif
}

void AMarkerCollection::UpdateMarkerLocations(const TMap<FName, FTransform>& UpdatedMarkerLocations)
{
	MarkerLocations = UpdatedMarkerLocations;
	RegisterWithSubsystem();
}

void AMarkerCollection::RegisterWithSubsystem()
{
	UMarkerSubsystem* Subsystem = GetWorld()->GetSubsystem<UMarkerSubsystem>();
	if (!IsValid(Subsystem))
	{
		return;
	}
	Subsystem->RegisterCollection(this);
}

void AMarkerCollection::BeginPlay()
{
	Super::BeginPlay();
	RegisterWithSubsystem();
}

void UMarkerSubsystem::CacheMarkerLocations()
{
#if WITH_EDITOR
	UWorldPartition* WorldPartition = GetWorld()->GetWorldPartition();
	if (!IsValid(WorldPartition))
	{
		return;
	}

	FWorldPartitionHelpers::ForEachActorDescInstance(WorldPartition, AActor::StaticClass(), [this, WorldPartition](const FWorldPartitionActorDescInstance* ActorDescInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionMarkerCollectionBuilder::PreRun - Reading actor desc \"%s\""), *ActorDescInstance->GetActorName().ToString());
		if (!ActorDescInstance->GetTags().Contains(UMarkerComponent::WithMarkerTag))
		{
			return true;
		}
		
		FWorldPartitionReference ActorReference(WorldPartition, ActorDescInstance->GetGuid());
		if (AActor* Actor = ActorReference.GetActor())
		{
			if (UMarkerComponent* Marker = Actor->GetComponentByClass<UMarkerComponent>())
			{
				UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionMarkerCollectionBuilder::PreRun - Caching marker \"%s\""), *Marker->MarkerID.ToString());
				MarkerLocations.Add(Marker->MarkerID, Marker->GetComponentTransform());
			}
		}
	
		return true;
	});
#endif
}

void UMarkerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMarkerSubsystem::RegisterCollection(AMarkerCollection* MarkerCollection)
{
	MarkerLocations.Append(MarkerCollection->MarkerLocations);
}
