// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

using UnrealBuildTool;

public class SpaceKitBullet : ModuleRules
{
	public SpaceKitBullet(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseSharedPCHs;
		bEnforceIWYU = true;

		bFasterWithoutUnity = true;

		PrivatePCHHeaderFile = "SpaceKitBullet.h";

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
