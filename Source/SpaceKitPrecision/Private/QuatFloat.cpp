// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "SpaceKitPrecision/Public/QuatFloat.h"
#include "SpaceKitPrecision/Public/RotatorFloat.h"

FQuatFloat FQuatFloat::Identity = FQuatFloat();

FQuatFloat::FQuatFloat(const FRotatorFloat& Rotator)
{
    // Precompute trigo ops
    const auto CosY = URealFloatMath::CosDeg(Rotator.Yaw * 0.5_fl);
    const auto SinY = URealFloatMath::SinDeg(Rotator.Yaw * 0.5_fl);
    const auto CosP = URealFloatMath::CosDeg(Rotator.Pitch * 0.5_fl);
    const auto SinP = URealFloatMath::SinDeg(Rotator.Pitch * 0.5_fl);
    const auto CosR = URealFloatMath::CosDeg(Rotator.Roll * 0.5_fl);
    const auto SinR = URealFloatMath::SinDeg(Rotator.Roll * 0.5_fl);
    
    // Build the corresponding quaternion
	X = SinR * CosP * CosY - CosR * SinP * SinY;
	Y = CosR * SinP * CosY + SinR * CosP * SinY;
	Z = CosR * CosP * SinY - SinR * SinP * CosY;
	W = CosR * CosP * CosY + SinR * SinP * SinY;
    *this = GetNormalized();
}

FQuatFloat UQuatFloatMath::ConvFQuatToQuatFloat(const FQuat& Quat)
{
    return FQuatFloat(Quat);
}

FQuat UQuatFloatMath::ConvQuatFloatToFQuat(const FQuatFloat& Quat)
{
    return Quat.ToFQuat();
}

FQuatFloat UQuatFloatMath::MakeQuatFloat(const FVectorFloat& Axis, const FRealFloat& Angle)
{
    return FQuatFloat(Axis, Angle);
}

void UQuatFloatMath::BreakQuatFloat(const FQuatFloat& Quat, FVectorFloat& Axis, FRealFloat& Angle)
{
    Quat.ToAxisAndAngle(Axis, Angle);
}

FQuatFloat UQuatFloatMath::InvQuat(const FQuatFloat& Quat)
{
    return Quat.Inverse();
}

bool UQuatFloatMath::QuatEqualsQuat(const FQuatFloat& First, const FQuatFloat& Second, const FRealFloat& Tolerance)
{
    return URealFloatMath::RealEqualsReal(First.X, Second.X, Tolerance)
        && URealFloatMath::RealEqualsReal(First.Y, Second.Y, Tolerance)
        && URealFloatMath::RealEqualsReal(First.Z, Second.Z, Tolerance)
        && URealFloatMath::RealEqualsReal(First.W, Second.W, Tolerance);
}

bool UQuatFloatMath::QuatNotEqualsQuat(const FQuatFloat& First, const FQuatFloat& Second, const FRealFloat& Tolerance)
{
    return !QuatEqualsQuat(First, Second, Tolerance);
}

FQuatFloat UQuatFloatMath::Slerp(FQuatFloat First, FQuatFloat Second, const FRealFloat& Alpha)
{
    // Source code from https://en.wikipedia.org/wiki/Slerp

    FRealFloat Dot = First.X * Second.X + First.Y * Second.Y + First.Z * Second.Z + First.W * Second.W;

    if (Dot < FRealFloat(0.f)) 
    {
        Second = FQuatFloat(-Second.X, -Second.X, -Second.X, -Second.X);
        Dot = -Dot;
    }

    if (Dot > 0.9995_fl)
    {
        return FQuatFloat(
            First.X + Alpha * (Second.X - First.X),
            First.Y + Alpha * (Second.Y - First.Y),
            First.Z + Alpha * (Second.Z - First.Z),
            First.W + Alpha * (Second.W - First.W)
        ).GetNormalized();
    }

    FRealFloat theta_0 = URealFloatMath::AcosRad(Dot);
    FRealFloat theta = theta_0 * Alpha;
    FRealFloat sin_theta = URealFloatMath::SinRad(theta);
    FRealFloat sin_theta_0 = URealFloatMath::SinRad(theta_0);

    FRealFloat s0 = URealFloatMath::CosRad(theta) - Dot * sin_theta / sin_theta_0;
    FRealFloat s1 = sin_theta / sin_theta_0;

    return FQuatFloat(
        (s0 * First.X) + (s1 * Second.X),
        (s0 * First.Y) + (s1 * Second.Y),
        (s0 * First.Z) + (s1 * Second.Z),
        (s0 * First.W) + (s1 * Second.W)
    );
}
