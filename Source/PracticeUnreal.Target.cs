// Practice Unreal by Stiner

using UnrealBuildTool;
using System.Collections.Generic;

public class PracticeUnrealTarget : TargetRules
{
	public PracticeUnrealTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "PracticeUnreal" } );
	}
}
