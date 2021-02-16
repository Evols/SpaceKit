// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "SpaceKitPrecisionEditor/Public/SpaceKitPrecisionEditor.h"
#include "SpaceKitPrecisionEditor/Public/RealGenericEditor.h"
#include "SpaceKitPrecisionEditor/Public/VectorGenericEditor.h"

#include "SpaceKitPrecision/Public/RealFixed.h"
#include "SpaceKitPrecision/Public/RealFloat.h"
#include "SpaceKitPrecision/Public/VectorFixed.h"

#include "Engine/Public/Engine.h"
#include "PropertyEditor/Public/PropertyEditorModule.h"
#include "Core/Public/Misc/CoreDelegates.h"
#include "VectorFloat.h"
#include "RotatorFloat.h"

#define LOCTEXT_NAMESPACE "FSpaceKitPrecisionEditorModule"

void FSpaceKitPrecisionEditorModule::StartupModule()
{
	// Generate callback to OnPostEngineInit
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FSpaceKitPrecisionEditorModule::OnPostEngineInit);
}

void FSpaceKitPrecisionEditorModule::OnPostEngineInit()
{
	RegisterCustomPropertiesLayout();
	RegisterPinFactories();
}

void FSpaceKitPrecisionEditorModule::RegisterCustomPropertiesLayout()
{
	auto& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(
		"RealFixed",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FRealStructCustomization<FRealFixed>::MakeInstance)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		"RealFloat",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FRealStructCustomization<FRealFloat>::MakeInstance)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		"VectorFixed",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FVectorGenericStructCustomization<FVectorFixed, FRealFixed>::MakeInstanceDefaults)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		"VectorFloat",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FVectorGenericStructCustomization<FVectorFloat, FRealFloat>::MakeInstanceDefaults)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		"RotatorFloat",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FVectorGenericStructCustomization<FRotatorFloat, FRealFloat>::MakeInstance,
			LOCTEXT("LabelRoll", "R"), LOCTEXT("LabelPitch", "P"), LOCTEXT("LabelYaw", "Y"),
			LOCTEXT("TooltipRoll", "Roll"), LOCTEXT("TooltipPitch", "Pitch"), LOCTEXT("TooltipYaw", "Yaw")
		)
	);

	PropertyModule.NotifyCustomizationModuleChanged();
}

// Pin factory
class FSpaceKitPrecisionGraphPanelPinFactory : public FGraphPanelPinFactory
{
	TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* InPin) const override
	{
		if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
		{
			if (UScriptStruct* PinStructType = Cast<UScriptStruct>(InPin->PinType.PinSubCategoryObject.Get()))
			{
				if (PinStructType->IsChildOf(FRealFixed::StaticStruct()))
				{
					return SNew(SRealGraphPin<FRealFixed>, InPin);
				}
				if (PinStructType->IsChildOf(FRealFloat::StaticStruct()))
				{
					return SNew(SRealGraphPin<FRealFloat>, InPin);
				}
			}
		}

		return nullptr;
	}
};

// Register pin factory
void FSpaceKitPrecisionEditorModule::RegisterPinFactories()
{
	// Register real_fixed literal
	const TSharedPtr<FSpaceKitPrecisionGraphPanelPinFactory> RealGraphPanelPinFactory = MakeShareable(new FSpaceKitPrecisionGraphPanelPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(RealGraphPanelPinFactory);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSpaceKitPrecisionEditorModule, SpaceKitPrecisionEditor)
