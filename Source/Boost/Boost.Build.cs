using System;
using System.IO;
using UnrealBuildTool;

public class Boost : ModuleRules
{
    public Boost(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = ModuleRules.PCHUsageMode.UseSharedPCHs;
        PrivatePCHHeaderFile = "Public/Boost.h";

        bEnforceIWYU = true;

        Type = ModuleType.External;

        // Add any include paths for the plugin
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
    }
}
