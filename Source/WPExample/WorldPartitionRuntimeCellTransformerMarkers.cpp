// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldPartitionRuntimeCellTransformerMarkers.h"
#include "MarkerComponent.h"

#if WITH_EDITOR
void UWorldPartitionRuntimeCellTransformerMarkers::PreTransform(ULevel* InLevel)
{
	Super::PreTransform(InLevel);

	UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionRuntimeCellTransformerMarkers::PreTransform"));
}

void UWorldPartitionRuntimeCellTransformerMarkers::Transform(ULevel* InLevel)
{
	Super::Transform(InLevel);

	UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionRuntimeCellTransformerMarkers::Transform"));

	TMap<FName, FTransform> MarkerLocations;
	for (TObjectPtr<AActor>& Actor : InLevel->Actors)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionRuntimeCellTransformerMarkers::Transform - Actor: %s"), *Actor.GetName());
		if (!IsValid(Actor))
		{
			continue;
		}

		UMarkerComponent* Marker = Actor->FindComponentByClass<UMarkerComponent>();
		if (!IsValid(Marker))
		{
			continue;
		}
			
		MarkerLocations.Add(Marker->MarkerID, Marker->GetComponentTransform());
		UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionRuntimeCellTransformerMarkers::Transform - Marker: %s"), *Marker->MarkerID.ToString());
	}

	if (MarkerLocations.IsEmpty())
	{
		return;
	}

	// static AMarkerManager* MarkerManager = nullptr;
	//
	// UWorld* World = GetWorld();
	//
	// // UWorld* PersistentWorld = World->PersistentLevel->GetWorld();
	// // AMarkerManager* MarkerManager;
	// // if (TActorIterator<AMarkerManager> It(PersistentWorld); It)
	// // {
	// // 	MarkerManager = *It;
	// // }
	// if (!IsValid(MarkerManager))
	// {
	// 	MarkerManager = NewObject<AMarkerManager>(InLevel);
	// 	MarkerManager->SetIsSpatiallyLoaded(false);
	// 	World->PersistentLevel->Actors.Add(MarkerManager);
	// }
	//
	// MarkerManager->MarkerLocations.Append(MarkerLocations);
	// MarkerManager->MarkPackageDirty();
	// UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionRuntimeCellTransformerMarkers::Transform - Manager: %s"), *MarkerManager->GetName());
}

void UWorldPartitionRuntimeCellTransformerMarkers::PostTransform(ULevel* InLevel)
{
	UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionRuntimeCellTransformerMarkers::PostTransform"));
	
	Super::PostTransform(InLevel);
}
#endif
