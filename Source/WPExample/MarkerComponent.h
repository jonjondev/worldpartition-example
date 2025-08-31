// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObjectDefinition.h"
#include "Components/SceneComponent.h"
#include "MarkerComponent.generated.h"

UCLASS()
class UDummySmartObjectBehaviorDefinition : public USmartObjectBehaviorDefinition
{
	GENERATED_BODY()
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WPEXAMPLE_API UMarkerComponent : public USceneComponent
{
	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA
	static const FName WithMarkerTag;
#endif
	
	UMarkerComponent();

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName MarkerID;

};
