// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldPartitionMarkerCollectionBuilder.h"
#include "EngineUtils.h"
#include "FileHelpers.h"
#include "ISourceControlModule.h"
#include "UObject/SavePackage.h"
#include "WorldPartition/WorldPartition.h"
#include "WorldPartition/WorldPartitionHelpers.h"
#include "WPExample/MarkerComponent.h"
#include "WPExample/MarkerSubsystem.h"

const FName UWorldPartitionMarkerCollectionBuilder::EditorBuildType = FName("MarkerCollections");

bool UWorldPartitionMarkerCollectionBuilder::CanBuildCollections(const UWorld* InWorld, FName BuildOption)
{
	return InWorld != nullptr && InWorld->IsPartitionedWorld();
}

#define LOCTEXT_NAMESPACE "MarkerCollection"

EEditorBuildResult UWorldPartitionMarkerCollectionBuilder::BuildCollections(UWorld* InWorld, FName BuildOption)
{
	check(InWorld)
	
	const FString& PackageToReloadOnSuccess = GetNameSafe(InWorld->GetPackage());

	// Try to provide complete Path, if we can't try with project name
	const FString ProjectPath = FPaths::IsProjectFilePathSet() ? FPaths::GetProjectFilePath() : FApp::GetProjectName();

	const ISourceControlProvider& SCCProvider = ISourceControlModule::Get().GetProvider();

	const FString Arguments = FString::Printf(TEXT("\"%s\" -run=WorldPartitionBuilderCommandlet %s %s -SCCProvider=%s"),
			*ProjectPath,
			*PackageToReloadOnSuccess,
			TEXT(" -AllowCommandletRendering -Builder=WorldPartitionMarkerCollectionBuilder -log=WPMarkerCollectionBuilderLog.txt -logcmds=\"LogMarkerCollection VeryVerbose\""),
			*SCCProvider.GetName().ToString());
		
	const bool bSuccess = FEditorBuildUtils::RunWorldPartitionBuilder(PackageToReloadOnSuccess,
		LOCTEXT("WorldPartitionBuildMarkerCollectionProgress", "Building marker collection..."),
		LOCTEXT("WorldPartitionBuildMarkerCollectionCancelled", "Building marker collection cancelled!"),
		LOCTEXT("WorldPartitionBuildMarkerCollectionFailed", "Errors occured during the build process, please refer to the logs ('WPMarkerCollectionBuilderLog.txt')."),
		Arguments);
	
	return bSuccess ? EEditorBuildResult::Success : EEditorBuildResult::Skipped;
}

#undef LOCTEXT_NAMESPACE

bool UWorldPartitionMarkerCollectionBuilder::PreRun(UWorld* World, FPackageSourceControlHelper& PackageHelper)
{
	UMarkerSubsystem* Subsystem = World->GetSubsystem<UMarkerSubsystem>();
	if (!IsValid(Subsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("UWorldPartitionMarkerCollectionBuilder::PreRun - Failed to find marker world subsystem!"));
		return false;
	}
	
	Subsystem->CacheMarkerLocations();
	CachedMarkerLocations = Subsystem->MarkerLocations;
	return true;
}

bool UWorldPartitionMarkerCollectionBuilder::RunInternal(UWorld* World, const FCellInfo& InCellInfo, FPackageSourceControlHelper& PackageHelper)
{
	UWorldPartition* WorldPartition = World->GetWorldPartition();
	if (!IsValid(WorldPartition))
	{
		UE_LOG(LogTemp, Error, TEXT("UWorldPartitionMarkerCollectionBuilder::RunInternal - Failed to find WorldPartition!"));
		return false;
	}

	if (TActorIterator<AMarkerCollection> It(World); It)
	{
		CachedMarkerCollection = *It;
		UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionMarkerCollectionBuilder::RunInternal - Found collection \"%s\""), *CachedMarkerCollection->GetName());
	}
	else
	{
		CachedMarkerCollection = World->SpawnActor<AMarkerCollection>();
		UE_LOG(LogTemp, Log, TEXT("UWorldPartitionMarkerCollectionBuilder::RunInternal - Spawned new collection \"%s\""), *CachedMarkerCollection->GetName());
	}
	CachedMarkerCollection->UpdateMarkerLocations(CachedMarkerLocations);
	UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionMarkerCollectionBuilder::RunInternal - Updated marker collection \"%s\""), *CachedMarkerCollection->GetName());
	return true;
}

bool UWorldPartitionMarkerCollectionBuilder::PostRun(UWorld* World, FPackageSourceControlHelper& PackageHelper, const bool bInRunSuccess)
{
	bool bErrorsEncountered = !bInRunSuccess;
	
	bool bSuccess= CachedMarkerCollection->MarkPackageDirty();
	if (!bSuccess)
	{
		bErrorsEncountered = true;
	}

	UPackage* Package = CachedMarkerCollection->GetPackage();
	ensure(Package->IsDirty());

	// Checkout package to save
	if (PackageHelper.UseSourceControl())
	{
		FEditorFileUtils::CheckoutPackages({Package}, nullptr, false);
	}
	else
	{
		// Remove read-only
		const FString PackageFilename = SourceControlHelpers::PackageFilename(Package);
		if (IPlatformFile::GetPlatformPhysical().FileExists(*PackageFilename))
		{
			if (!IPlatformFile::GetPlatformPhysical().SetReadOnly(*PackageFilename, false))
			{
				UE_LOG(LogTemp, Error, TEXT("Error setting %s writable"), *PackageFilename);
				bErrorsEncountered = true;
			}
		}
	}

	// Save packages
	UE_LOG(LogTemp, Log, TEXT("   Saving package  %s."), *Package->GetName());
	const FString PackageFileName = SourceControlHelpers::PackageFilename(Package);
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Standalone;
	SaveArgs.SaveFlags = SAVE_None;
	if (!UPackage::SavePackage(Package, nullptr, *PackageFileName, SaveArgs))
	{
		UE_LOG(LogTemp, Error, TEXT("Error saving package %s."), *Package->GetName());
		bErrorsEncountered = true;
	}

	// Add new packages to source control
	UE_LOG(LogTemp, Log, TEXT("Adding package to source control."));

	if (!PackageHelper.AddToSourceControl(Package))
	{
		UE_LOG(LogTemp, Error, TEXT("Error adding package %s to source control."), *Package->GetName());
		bErrorsEncountered = true;
	}
	
	CachedMarkerCollection = nullptr;

	if (bErrorsEncountered)
	{
		UE_LOG(LogTemp, Error, TEXT("UWorldPartitionMarkerCollectionBuilder::PostRun - Finalised build with errors!"));
		return false;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UWorldPartitionMarkerCollectionBuilder::PostRun - Finalised build successfully!"));
	return true;
}
