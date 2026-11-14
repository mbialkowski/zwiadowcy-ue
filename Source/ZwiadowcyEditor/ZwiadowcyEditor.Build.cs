using UnrealBuildTool;

public class ZwiadowcyEditor : ModuleRules
{
	public ZwiadowcyEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"Zwiadowcy"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"UnrealEd",
			"Json",
			"JsonUtilities"
		});
	}
}
