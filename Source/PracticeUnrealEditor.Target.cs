// Practice Unreal by Stiner

using UnrealBuildTool;

public class PracticeUnrealEditorTarget : TargetRules
{
    public PracticeUnrealEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "PracticeUnreal" });
    }
}
