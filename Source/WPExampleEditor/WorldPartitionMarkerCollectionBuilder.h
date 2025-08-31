// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorBuildUtils.h"
#include "WorldPartition/WorldPartitionBuilder.h"
#include "WorldPartitionMarkerCollectionBuilder.generated.h"

class AMarkerCollection;

UCLASS()
class WPEXAMPLEEDITOR_API UWorldPartitionMarkerCollectionBuilder : public UWorldPartitionBuilder
{
	GENERATED_BODY()

public:

	static const FName EditorBuildType;

	static bool CanBuildCollections(const UWorld* InWorld, FName BuildOption);
	
	static EEditorBuildResult BuildCollections(UWorld* InWorld, FName BuildOption);

protected:
	
	virtual bool RequiresCommandletRendering() const override { return false; }
	
	virtual ELoadingMode GetLoadingMode() const override { return EntireWorld; }
	
	virtual bool PreRun(UWorld* World, FPackageSourceControlHelper& PackageHelper) override;
	
	virtual bool RunInternal(UWorld* World, const FCellInfo& InCellInfo, FPackageSourceControlHelper& PackageHelper) override;
	
	virtual bool PostRun(UWorld* World, FPackageSourceControlHelper& PackageHelper, const bool bInRunSuccess) override;

	UPROPERTY(Transient)
	TMap<FName, FTransform> CachedMarkerLocations;

	UPROPERTY(Transient)
	AMarkerCollection* CachedMarkerCollection = nullptr;
	
};
