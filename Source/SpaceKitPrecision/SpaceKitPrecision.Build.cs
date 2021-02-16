// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

using UnrealBuildTool;

public class SpaceKitPrecision : ModuleRules
{
	public SpaceKitPrecision(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseSharedPCHs;
		bEnforceIWYU = true;

		PrivatePCHHeaderFile = "SpaceKitPrecision.h";

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Boost" });
	}
}
