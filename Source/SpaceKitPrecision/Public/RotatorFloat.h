// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "SpaceKitPrecision/Public/RealFloat.h"
#include "SpaceKitPrecision/Public/VectorFloat.h"

#include "RotatorFloat.generated.h"

struct FQuatFloat;

/*
 * Similar to an FRotator, but using big float reals instead of floats.
 */
USTRUCT(BlueprintType)
struct SPACEKITPRECISION_API FRotatorFloat
{
    GENERATED_BODY()

public:

	UPROPERTY()
    FRealFloat Yaw;

	UPROPERTY()
    FRealFloat Pitch;

	UPROPERTY()
    FRealFloat Roll;

    static FRotatorFloat Identity;

    FRotatorFloat()
        : Yaw(0.0), Pitch(0.0), Roll(0.0)
    {
    }

    FRotatorFloat(FRealFloat InYaw, FRealFloat InPitch, FRealFloat InRoll)
        : Yaw(InYaw), Pitch(InPitch), Roll(InRoll)
    {
    }

    explicit FRotatorFloat(const FRotator& InRot)
        : Yaw(InRot.Yaw), Pitch(InRot.Pitch), Roll(InRot.Roll)
    {
    }

    // Builds a rotator from a quaternion
    explicit FRotatorFloat(const FQuatFloat& Rotator);

// Rotator math
public:
	
    FRotatorFloat operator+(const FRotatorFloat& Other) const
    {
        return FRotatorFloat(Yaw + Other.Yaw, Pitch + Other.Pitch, Roll + Other.Roll);
    }

    FRotatorFloat& operator+=(const FRotatorFloat& Other)
    {
        return *this = *this + Other;
    }

    FRotatorFloat operator-(const FRotatorFloat& Other) const
    {
        return FRotatorFloat(Yaw - Other.Yaw, Pitch - Other.Pitch, Roll - Other.Roll);
    }

    FRotatorFloat& operator-=(const FRotatorFloat& Other)
    {
        return *this = *this - Other;
    }

    FRotatorFloat operator*(const FRotatorFloat& Other) const
    {
        return FRotatorFloat(Yaw * Other.Yaw, Pitch * Other.Pitch, Roll * Other.Roll);
    }

    FRotatorFloat& operator*=(const FRotatorFloat& Other)
    {
        return *this = *this * Other;
    }

    FRotatorFloat operator*(FRealFloat Other) const
    {
        return FRotatorFloat(Yaw * Other, Pitch * Other, Roll * Other);
    }

    FRotatorFloat& operator*=(FRealFloat Other)
    {
        return *this = *this * Other;
    }

    FRotatorFloat operator/(const FRotatorFloat& Other) const
    {
        return FRotatorFloat(Yaw / Other.Yaw, Pitch / Other.Pitch, Roll / Other.Roll);
    }

    FRotatorFloat& operator/=(const FRotatorFloat& Other)
    {
        return *this = *this / Other;
    }

    FRotatorFloat operator/(FRealFloat Other) const
    {
        return FRotatorFloat(Yaw / Other, Pitch / Other, Roll / Other);
    }

    FRotatorFloat& operator/=(FRealFloat Other)
    {
        return *this = *this / Other;
    }

    FRotatorFloat operator-() const
    {
        return FRotatorFloat(-Yaw, -Pitch, -Roll);
    }

    FRotator ToFRotator() const
    {
        return FRotator(Pitch.ToFloat(), Yaw.ToFloat(), Roll.ToFloat());
    }

    bool Equals(const FRotatorFloat& Other, FRealFloat Tolerance = FRealFloat(1.0e-8)) const
    {
        return (*this - Other).GetAbsSum() <= Tolerance;
    }

    bool operator==(const FRotatorFloat& Other) const
    {
        return Equals(Other);
    }

    bool operator!=(const FRotatorFloat& Other) const
    {
        return ! (*this == Other);
    }

    // Rotates a given vector by this quaternion
    FVectorFloat RotateVector(const FVectorFloat& Vec) const;

    // Rotates backward a given vector by this quaternion, so that for a given quaternion Q and a given vector V, UnrotateVector(RotateVector(V)) = V
    FVectorFloat UnrotateVector(const FVectorFloat& Vec) const;

    FRealFloat GetAbsSum() const
    {
        return URealFloatMath::Abs(Yaw) + URealFloatMath::Abs(Pitch) + URealFloatMath::Abs(Roll);
    }

    FString ToString() const
    {
        return FString::Printf(TEXT("(Yaw=%s,Pitch=%s,Roll=%s)"), *Yaw.ToString(), *Pitch.ToString(), *Roll.ToString());
    }

    FRealFloat& GetAxis(EAxis::Type Axis)
    {
        return Axis == EAxis::Z ? Yaw : Axis == EAxis::Y ? Pitch : Roll;
    }

    const FRealFloat& GetAxis(EAxis::Type Axis) const
    {
        return Axis == EAxis::Z ? Yaw : Axis == EAxis::Y ? Pitch : Roll;
    }
};

/**
 * Blueprints math library for RotatorFloat
 */
UCLASS(BlueprintType, Abstract)
class SPACEKITPRECISION_API URotatorFloatMath : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
	
// Basic conversions
public:

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta = (DisplayName = "FRotator to RotatorFloat", CompactNodeTitle = "->", BlueprintAutocast))
    static FRotatorFloat ConvFRotatorToRotatorFloat(const FRotator& InRot);

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta = (DisplayName = "RotatorFloat to FRotator", CompactNodeTitle = "->", BlueprintAutocast))
    static FRotator ConvRotatorFloatToFRotator(const FRotatorFloat& InRot);
    
    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta = (DisplayName = "Make RotatorFloat", CompactNodeTitle = "Make"))
    static FRotatorFloat MakeRotatorFloat(const FRealFloat& Yaw, const FRealFloat& Pitch, const FRealFloat& Roll);

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta = (DisplayName = "Break RotatorFloat", CompactNodeTitle = "Make"))
    static void BreakRotatorFloat(const FRotatorFloat& Rot, FRealFloat& Yaw, FRealFloat& Pitch, FRealFloat& Roll);

// Basic RotFloat math
public:

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta=(DisplayName = "RotFloat + RotFloat", CompactNodeTitle="+"))
    static FRotatorFloat RotPlusRot(const FRotatorFloat& First, const FRotatorFloat& Second);

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta=(DisplayName = "RotFloat - RotFloat", CompactNodeTitle="-"))
    static FRotatorFloat RotMinusRot(const FRotatorFloat& First, const FRotatorFloat& Second);

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta=(DisplayName = "RotFloat * RealFloat", CompactNodeTitle="*"))
    static FRotatorFloat RotMultReal(const FRotatorFloat& First, const FRealFloat& Second);

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta = (DisplayName = "RotFloat * RotFloat", CompactNodeTitle = "*"))
    static FRotatorFloat RotMultRot(const FRotatorFloat& First, const FRotatorFloat& Second);

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta=(DisplayName = "RotFloat / RealFloat", CompactNodeTitle="/"))
    static FRotatorFloat RotDivReal(const FRotatorFloat& First, const FRealFloat& Second);

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta = (DisplayName = "RotFloat / RotFloat", CompactNodeTitle = "/"))
    static FRotatorFloat RotDivRot(const FRotatorFloat& First, const FRotatorFloat& Second);

    UFUNCTION(BlueprintPure, category = "RotatorFloat", meta = (DisplayName = "RotFloat == RotFloat", CompactNodeTitle = "=="))
    static bool RotEqualsRot(const FRotatorFloat& First, const FRotatorFloat& Second, const FRealFloat& Tolerance);

	UFUNCTION(BlueprintPure, category = "RotatorFloat", meta = (DisplayName = "RotFloat != RotFloat", CompactNodeTitle = "!="))
    static bool RotNotEqualsRot(const FRotatorFloat& First, const FRotatorFloat& Second, const FRealFloat& Tolerance);

};
