// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldPartition/WorldPartitionRuntimeCellTransformer.h"
#include "WorldPartitionRuntimeCellTransformerMarkers.generated.h"

UCLASS()
class WPEXAMPLE_API UWorldPartitionRuntimeCellTransformerMarkers : public UWorldPartitionRuntimeCellTransformer
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual void PreTransform(ULevel* InLevel) override;
    virtual void Transform(ULevel* InLevel) override;
	virtual void PostTransform(ULevel* InLevel) override;
#endif

};
