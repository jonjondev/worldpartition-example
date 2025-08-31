using UnrealBuildTool;

public class WPExampleEditor : ModuleRules
{
    public WPExampleEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "WPExample",
                "EditorSubsystem",
                "UnrealEd",
                "SourceControl"
            }
        );
    }
}