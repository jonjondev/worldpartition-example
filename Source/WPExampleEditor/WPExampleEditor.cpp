#include "WPExampleEditor.h"

#include "EditorBuildUtils.h"
#include "WorldPartitionMarkerCollectionBuilder.h"

#define LOCTEXT_NAMESPACE "FWPExampleEditorModule"

void FWPExampleEditorModule::StartupModule()
{
	FEditorBuildUtils::RegisterCustomBuildType(
		UWorldPartitionMarkerCollectionBuilder::EditorBuildType,
		FCanDoEditorBuildDelegate::CreateStatic(&UWorldPartitionMarkerCollectionBuilder::CanBuildCollections),
		FDoEditorBuildDelegate::CreateStatic(&UWorldPartitionMarkerCollectionBuilder::BuildCollections),
		NAME_None,
		LOCTEXT("BuildMarkers", "Build Marker Collections"),
		LOCTEXT("Gameplay", "Gameplay"),
		false);
}

void FWPExampleEditorModule::ShutdownModule()
{
	FEditorBuildUtils::UnregisterCustomBuildType(UWorldPartitionMarkerCollectionBuilder::EditorBuildType);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FWPExampleEditorModule, WPExampleEditor)