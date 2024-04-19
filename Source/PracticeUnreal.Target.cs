// Practice Unreal by Stiner

using UnrealBuildTool;

public class PracticeUnrealTarget : TargetRules
{
    public PracticeUnrealTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "PracticeUnreal" });
    }
}
