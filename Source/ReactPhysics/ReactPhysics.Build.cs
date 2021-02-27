using System;
using System.IO;
using UnrealBuildTool;

public class ReactPhysics : ModuleRules
{
    public ReactPhysics(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = ModuleRules.PCHUsageMode.NoSharedPCHs;
		MinSourceFilesForUnityBuildOverride = 5000000;
		PrivatePCHHeaderFile = "ReactPhysics.h";

        bEnforceIWYU = true;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"SpaceKitPrecision",
		});
	}
}
