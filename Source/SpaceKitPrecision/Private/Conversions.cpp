// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "SpaceKitPrecision/Public/Conversions.h"

void UPrecisionConversionMath::ConvRotatorFloatToQuatFloat(const FRotatorFloat& Rot, FQuatFloat& Quat)
{
    Quat = FQuatFloat(Rot);
}

void UPrecisionConversionMath::ConvQuatFloatToRotatorFloat(const FQuatFloat& Quat, FRotatorFloat& Rot)
{
    Rot = FRotatorFloat(Quat);
}
