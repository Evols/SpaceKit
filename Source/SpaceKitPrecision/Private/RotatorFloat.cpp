// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "SpaceKitPrecision/Public/RotatorFloat.h"
#include "SpaceKitPrecision/Public/QuatFloat.h"

FRotatorFloat FRotatorFloat::Identity = FRotatorFloat();

FRotatorFloat::FRotatorFloat(const FQuatFloat& Rotator)
{
    // Roll (X-axis rotation)
    const FRealFloat sinr_cosp = 2_fl * (Rotator.W * Rotator.X + Rotator.Y * Rotator.Z);
    const FRealFloat cosr_cosp = 1_fl - 2_fl * (Rotator.X * Rotator.X + Rotator.Y * Rotator.Y);
    Roll = URealFloatMath::Atan2Deg(sinr_cosp, cosr_cosp);

    // Pitch (Y-axis rotation)
    const FRealFloat sinp = 2_fl * (Rotator.W * Rotator.Y - Rotator.Z * Rotator.X);
    if (URealFloatMath::Abs(sinp) >= 1_fl)
    {
        Pitch = URealFloatMath::Sign(sinp) * 90_fl; // Use 90 degrees if out of range
    }
    else
    {
        Pitch = URealFloatMath::AsinDeg(sinp);
    }

    // Yaw (Z-axis rotation)
    const FRealFloat siny_cosp = 2_fl * (Rotator.W * Rotator.Z + Rotator.X * Rotator.Y);
    const FRealFloat cosy_cosp = 1_fl - 2_fl * (Rotator.Y * Rotator.Y + Rotator.Z * Rotator.Z);
    Yaw = URealFloatMath::Atan2Deg(siny_cosp, cosy_cosp);
}

FVectorFloat FRotatorFloat::RotateVector(const FVectorFloat& Vec) const
{
    return FQuatFloat(*this).RotateVector(Vec);
}

FVectorFloat FRotatorFloat::UnrotateVector(const FVectorFloat& Vec) const
{
    return FQuatFloat(*this).UnrotateVector(Vec);
}

FRotatorFloat URotatorFloatMath::ConvFRotatorToRotatorFloat(const FRotator& InRot)
{
	return FRotatorFloat(InRot);
}

FRotator URotatorFloatMath::ConvRotatorFloatToFRotator(const FRotatorFloat& InRot)
{
	return InRot.ToFRotator();
}

FRotatorFloat URotatorFloatMath::MakeRotatorFloat(const FRealFloat& Yaw, const FRealFloat& Pitch, const FRealFloat& Roll)
{
	return FRotatorFloat(Yaw, Pitch, Roll);
}

void URotatorFloatMath::BreakRotatorFloat(const FRotatorFloat& Rot, FRealFloat& Yaw, FRealFloat& Pitch, FRealFloat& Roll)
{
	Yaw = Rot.Yaw;
	Pitch = Rot.Pitch;
	Roll = Rot.Roll;
}

FRotatorFloat URotatorFloatMath::RotPlusRot(const FRotatorFloat& First, const FRotatorFloat& Second)
{
	return First + Second;
}

FRotatorFloat URotatorFloatMath::RotMinusRot(const FRotatorFloat& First, const FRotatorFloat& Second)
{
	return First - Second;
}

FRotatorFloat URotatorFloatMath::RotMultReal(const FRotatorFloat& First, const FRealFloat& Second)
{
	return First * Second;
}

FRotatorFloat URotatorFloatMath::RotMultRot(const FRotatorFloat& First, const FRotatorFloat& Second)
{
	return First * Second;
}

FRotatorFloat URotatorFloatMath::RotDivReal(const FRotatorFloat& First, const FRealFloat& Second)
{
	return First / Second;
}

FRotatorFloat URotatorFloatMath::RotDivRot(const FRotatorFloat& First, const FRotatorFloat& Second)
{
	return First / Second;
}

bool URotatorFloatMath::RotEqualsRot(const FRotatorFloat& First, const FRotatorFloat& Second, const FRealFloat& Tolerance)
{
	return First.Equals(Second, Tolerance);
}

bool URotatorFloatMath::RotNotEqualsRot(const FRotatorFloat& First, const FRotatorFloat& Second, const FRealFloat& Tolerance)
{
	return !URotatorFloatMath::RotEqualsRot(First, Second, Tolerance);
}
