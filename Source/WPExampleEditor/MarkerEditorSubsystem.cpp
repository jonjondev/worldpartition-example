// Fill out your copyright notice in the Description page of Project Settings.

#include "MarkerEditorSubsystem.h"

#include "EngineUtils.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "WPExample/MarkerSubsystem.h"
#include "WorldPartition/WorldPartition.h"

void UMarkerEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	FWorldDelegates::OnPIEStarted.AddUObject(this, &UMarkerEditorSubsystem::OnPIEStarted);
	FWorldDelegates::OnPIEMapReady.AddUObject(this, &UMarkerEditorSubsystem::OnPIEMapReady);
}

void UMarkerEditorSubsystem::OnPIEStarted(UGameInstance* GameInstance)
{
	UUnrealEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
	if (!IsValid(EditorSubsystem))
	{
		return;
	}
	
	UWorld* World = EditorSubsystem->GetEditorWorld();
	if (!IsValid(World))
	{
		return;
	}

	UMarkerSubsystem* Subsystem = World->GetSubsystem<UMarkerSubsystem>();
	if (!IsValid(Subsystem))
	{
		return;
	}
	
	Subsystem->CacheMarkerLocations();
	CachedMarkerLocations = Subsystem->MarkerLocations;
}

void UMarkerEditorSubsystem::OnPIEMapReady(UGameInstance* GameInstance)
{
	UWorld* World = GameInstance->GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	UWorldPartition* WorldPartition = World->GetWorldPartition();
	if (!IsValid(WorldPartition))
	{
		return;
	}

	AMarkerCollection* MarkerCollection;
	if (TActorIterator<AMarkerCollection> It(World); It)
	{
		MarkerCollection = *It;
	}
	else
	{
		MarkerCollection = World->SpawnActor<AMarkerCollection>();
	}
	MarkerCollection->UpdateMarkerLocations(CachedMarkerLocations);
}
