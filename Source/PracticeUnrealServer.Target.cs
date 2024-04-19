// Practice Unreal by Stiner

using UnrealBuildTool;
using System.Collections.Generic;
using System;

public class PracticeUnrealServerTarget : TargetRules
{
    public PracticeUnrealServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange(new string[] { "PracticeUnreal" });
    }
}
