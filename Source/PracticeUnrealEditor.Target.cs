// Practice Unreal by Stiner

using UnrealBuildTool;
using System.Collections.Generic;

public class PracticeUnrealEditorTarget : TargetRules
{
	public PracticeUnrealEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "PracticeUnreal" } );
	}
}
