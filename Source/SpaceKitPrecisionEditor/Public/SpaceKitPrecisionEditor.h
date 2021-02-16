// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "CoreMinimal.h"

// SpaceKit Precision Editor module
class FSpaceKitPrecisionEditorModule : public IModuleInterface
{
public:

	void StartupModule() override;
	
	// Used to initialized things that need the engine to be initialized, such as the two functions below
	void OnPostEngineInit();

	// Register custom stuff
	void RegisterCustomPropertiesLayout();
	void RegisterPinFactories();

};
