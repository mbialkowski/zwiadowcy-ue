using UnrealBuildTool;

public class ZwiadowcyUETarget : TargetRules
{
	public ZwiadowcyUETarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("Zwiadowcy");
	}
}
