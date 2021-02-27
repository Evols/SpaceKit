using System;
using System.IO;
using UnrealBuildTool;

public class ReactPhysics : ModuleRules
{
    public ReactPhysics(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = ModuleRules.PCHUsageMode.UseSharedPCHs;
        PrivatePCHHeaderFile = "ReactPhysics.h";

		// MinSourceFilesForUnityBuildOverride = System.Int32.MaxValue;

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
