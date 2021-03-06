using System;
using System.IO;
using UnrealBuildTool;

public class ReactPhysics : ModuleRules
{
    public ReactPhysics(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = ModuleRules.PCHUsageMode.UseSharedPCHs;
        PrivatePCHHeaderFile = "ReactPhysics.h";

		bEnableUndefinedIdentifierWarnings = false;

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
