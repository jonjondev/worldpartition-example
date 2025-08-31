// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "MarkerEditorSubsystem.generated.h"

UCLASS()
class WPEXAMPLEEDITOR_API UMarkerEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	
	void OnPIEStarted(UGameInstance* GameInstance);

	void OnPIEMapReady(UGameInstance* GameInstance);

	UPROPERTY(Transient)
	TMap<FName, FTransform> CachedMarkerLocations;
};
