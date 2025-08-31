// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MarkerSubsystem.generated.h"

class UMarkerComponent;
class AMarkerManager;

UCLASS()
class WPEXAMPLE_API AMarkerCollection : public AActor
{
	GENERATED_BODY()

public:
	
	AMarkerCollection();

	void UpdateMarkerLocations(const TMap<FName, FTransform>& UpdatedMarkerLocations);
	
	void RegisterWithSubsystem();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FName, FTransform> MarkerLocations;

	friend class UMarkerSubsystem;
	
};

UCLASS()
class WPEXAMPLE_API UMarkerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	void CacheMarkerLocations();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void RegisterCollection(AMarkerCollection* MarkerCollection);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	TMap<FName, FTransform> MarkerLocations;
};
