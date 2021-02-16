// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

using UnrealBuildTool;
using System;
using System.IO;

public class SpaceKitPrecisionEditor : ModuleRules
{
	public SpaceKitPrecisionEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseSharedPCHs;
		bEnforceIWYU = true;

		PrivatePCHHeaderFile = "SpaceKitPrecisionEditor.h";

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"SpaceKitPrecision",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"InputCore",
			"Engine",
			"CoreUObject",
            "Slate",
            "SlateCore",
            "EditorStyle",
			"BlueprintGraph",
			"GraphEditor",
			"UnrealEd",
		});
	}
}
