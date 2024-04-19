// Practice Unreal by Stiner

using UnrealBuildTool;

public class PracticeUnrealServerTarget : TargetRules
{
    public PracticeUnrealServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "PracticeUnreal" });
    }
}
