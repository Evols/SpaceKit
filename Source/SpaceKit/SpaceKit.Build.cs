// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

using UnrealBuildTool;

public class SpaceKit : ModuleRules
{
	public SpaceKit(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseSharedPCHs;
		bEnforceIWYU = true;

		PrivatePCHHeaderFile = "SpaceKit.h";

		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"SpaceKitPrecision",
			"ReactPhysics",
			"Boost",
		});
	}
}
