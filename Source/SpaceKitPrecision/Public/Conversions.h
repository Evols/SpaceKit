// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "RotatorFloat.h"
#include "QuatFloat.h"
#include "VectorFixed.h"

#include "Conversions.generated.h"

/**
 * Blueprints library for conversions. It was put here, in a separate file, to have a cleaner dependency tree
 */
UCLASS(BlueprintType, Abstract)
class SPACEKITPRECISION_API UPrecisionConversionMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

// Basic conversions
public:
	
    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta = (DisplayName = "RotatorFloat to QuatFloat", CompactNodeTitle = "->", BlueprintAutocast))
    static void ConvRotatorFloatToQuatFloat(const FRotatorFloat& Rot, FQuatFloat& Quat);
    
    UFUNCTION(BlueprintPure, category = "QuatFloat", meta = (DisplayName = "QuatFloat to RotatorFloat", CompactNodeTitle = "->", BlueprintAutocast))
    static void ConvQuatFloatToRotatorFloat(const FQuatFloat& Quat, FRotatorFloat& Rot);

};
