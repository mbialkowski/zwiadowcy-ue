using UnrealBuildTool;

public class ZwiadowcyUEEditorTarget : TargetRules
{
	public ZwiadowcyUEEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.AddRange(new string[] { "Zwiadowcy", "ZwiadowcyEditor" });
	}
}
