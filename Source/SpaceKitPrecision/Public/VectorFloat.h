// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "SpaceKitPrecision/Public/RealFloat.h"

#include "VectorFloat.generated.h"


struct FVectorFixed;
/*
 * Similar to an FVector, but using big float reals instead of floats.
 */
USTRUCT(BlueprintType)
struct SPACEKITPRECISION_API FVectorFloat
{
    GENERATED_BODY()

public:

    UPROPERTY()
    FRealFloat X;

    UPROPERTY()
    FRealFloat Y;

    UPROPERTY()
    FRealFloat Z;

    static FVectorFloat Identity;

    FVectorFloat()
        : X(0.0), Y(0.0), Z(0.0)
    {
    }

    FVectorFloat(FRealFloat InX, FRealFloat InY, FRealFloat InZ)
        : X(InX), Y(InY), Z(InZ)
    {
    }

    FVectorFloat(double InX, double InY, double InZ)
        : X(InX), Y(InY), Z(InZ)
    {
    }

    explicit FVectorFloat(const FVector& InVec)
        : X(InVec.X), Y(InVec.Y), Z(InVec.Z)
    {
    }

// Vector math
public:
	
    FVectorFloat operator+(const FVectorFloat& Other) const
    {
        return FVectorFloat(X + Other.X, Y + Other.Y, Z + Other.Z);
    }

    FVectorFloat& operator+=(const FVectorFloat& Other)
    {
        return *this = *this + Other;
    }

    FVectorFloat operator-(const FVectorFloat& Other) const
    {
        return FVectorFloat(X - Other.X, Y - Other.Y, Z - Other.Z);
    }

    FVectorFloat& operator-=(const FVectorFloat& Other)
    {
        return *this = *this - Other;
    }

    FVectorFloat operator*(const FVectorFloat& Other) const
    {
        return FVectorFloat(X * Other.X, Y * Other.Y, Z * Other.Z);
    }

    FVectorFloat& operator*=(const FVectorFloat& Other)
    {
        return *this = *this * Other;
    }

    FVectorFloat operator*(FRealFloat Other) const
    {
        return FVectorFloat(X * Other, Y * Other, Z * Other);
    }

    FVectorFloat& operator*=(FRealFloat Other)
    {
        return *this = *this * Other;
    }

    FVectorFloat operator/(const FVectorFloat& Other) const
    {
        return FVectorFloat(X / Other.X, Y / Other.Y, Z / Other.Z);
    }

    FVectorFloat& operator/=(const FVectorFloat& Other)
    {
        return *this = *this / Other;
    }

    FVectorFloat operator/(FRealFloat Other) const
    {
        return FVectorFloat(X / Other, Y / Other, Z / Other);
    }

    FVectorFloat& operator/=(FRealFloat Other)
    {
        return *this = *this / Other;
    }

    FVectorFloat operator-() const
    {
        return FVectorFloat(-X, -Y, -Z);
    }

    FVector ToFVector() const
    {
        return FVector(X.ToFloat(), Y.ToFloat(), Z.ToFloat());
    }

    static FRealFloat DotProduct(const FVectorFloat& Vec, const FVectorFloat& Other)
    {
        return Vec.X * Other.X + Vec.Y * Other.Y + Vec.Z * Other.Z;
    }

    FRealFloat operator|(const FVectorFloat& Other) const
    {
        return DotProduct(*this, Other);
    }

    static FVectorFloat CrossProduct(const FVectorFloat& Vec, const FVectorFloat& Other)
    {
        return FVectorFloat(Vec.Y * Other.Z - Vec.Z * Other.Y, Vec.Z * Other.X - Vec.X * Other.Z, Vec.X * Other.Y - Vec.Y * Other.X);
    }

    FVectorFloat operator^(const FVectorFloat& Other) const
    {
        return CrossProduct(*this, Other);
    }

    bool Equals(const FVectorFloat& Other, FRealFloat Tolerance = FRealFloat(1.0e-8)) const
    {
        return (*this - Other).GetAbsSum() <= Tolerance;
    }

    bool operator==(const FVectorFloat& Other) const
    {
        return Equals(Other);
    }

    bool operator!=(const FVectorFloat& Other) const
    {
        return ! (*this == Other);
    }

    FRealFloat SizeSquared() const
    {
        return DotProduct(*this, *this);
    }

    FRealFloat Size() const
    {
        const auto SizeSq = SizeSquared();
        return URealFloatMath::Sqrt(SizeSq);
    }

    FVectorFloat GetNormal(FRealFloat Tolerance = FRealFloat(1.0e-8)) const
    {
        const FRealFloat ThisSize = Size();
        if (ThisSize < Tolerance) return Identity;
        return *this / ThisSize;
    }

    FRealFloat GetAbsSum() const
    {
        return URealFloatMath::Abs(X) + URealFloatMath::Abs(Y) + URealFloatMath::Abs(Z);
    }

    FString ToString() const
    {
        return FString::Printf(TEXT("(X=%s,Y=%s,Z=%s)"), *X.ToString(), *Y.ToString(), *Z.ToString());
    }

    FRealFloat& GetAxis(EAxis::Type Axis)
    {
        return Axis == EAxis::X ? X : Axis == EAxis::Y ? Y : Z;
    }

    const FRealFloat& GetAxis(EAxis::Type Axis) const
    {
        return Axis == EAxis::X ? X : Axis == EAxis::Y ? Y : Z;
    }
};


inline FVectorFloat operator*(FRealFloat Other, const FVectorFloat& Vec)
{
    return FVectorFloat(Vec.X * Other, Vec.Y * Other, Vec.Z * Other);
}


/**
 * Blueprints math library for VectorFloat
 */
UCLASS(BlueprintType, Abstract)
class SPACEKITPRECISION_API UVectorFloatMath : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
	
// Basic conversions
public:

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "FVector to VectorFloat", CompactNodeTitle = "->", BlueprintAutocast))
    static FVectorFloat ConvFVectorToVectorFloat(const FVector& InVec);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "VectorFloat to FVector", CompactNodeTitle = "->", BlueprintAutocast))
    static FVector ConvVectorFloatToFVector(const FVectorFloat& InVec);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "Make VectorFloat", CompactNodeTitle = "Make"))
    static FVectorFloat MakeVectorFloat(const FRealFloat& X, const FRealFloat& Y, const FRealFloat& Z);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "Break VectorFloat", CompactNodeTitle = "Make"))
    static void BreakVectorFloat(const FVectorFloat& Vec, FRealFloat& X, FRealFloat& Y, FRealFloat& Z);

// Basic VecFloat math
public:

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta=(DisplayName = "VecFloat + VecFloat", CompactNodeTitle="+"))
    static FVectorFloat VecPlusVec(const FVectorFloat& First, const FVectorFloat& Second);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta=(DisplayName = "VecFloat - VecFloat", CompactNodeTitle="-"))
    static FVectorFloat VecMinusVec(const FVectorFloat& First, const FVectorFloat& Second);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta=(DisplayName = "VecFloat * RealFloat", CompactNodeTitle="*"))
    static FVectorFloat VecMultReal(const FVectorFloat& First, const FRealFloat& Second);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "VecFloat * VecFloat", CompactNodeTitle = "*"))
    static FVectorFloat VecMultVec(const FVectorFloat& First, const FVectorFloat& Second);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta=(DisplayName = "VecFloat / RealFloat", CompactNodeTitle="/"))
    static FVectorFloat VecDivReal(const FVectorFloat& First, const FRealFloat& Second);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "VecFloat / VecFloat", CompactNodeTitle = "/"))
    static FVectorFloat VecDivVec(const FVectorFloat& First, const FVectorFloat& Second);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "VecFloat == VecFloat", CompactNodeTitle = "=="))
    static bool VecEqualsVec(const FVectorFloat& First, const FVectorFloat& Second, const FRealFloat& Tolerance);

	UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "VecFloat != VecFloat", CompactNodeTitle = "!="))
    static bool VecNotEqualsVec(const FVectorFloat& First, const FVectorFloat& Second, const FRealFloat& Tolerance);

// Advanced RealFloat math
public:
    
    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "VecFloat DotProduct", CompactNodeTitle = "|"))
    static FRealFloat DotProduct(const FVectorFloat& First, const FVectorFloat& Second);
    
    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "VecFloat CrossProduct", CompactNodeTitle = "^"))
    static FVectorFloat CrossProduct(const FVectorFloat& First, const FVectorFloat& Second);

    UFUNCTION(BlueprintPure, category = "VectorFloat", meta = (DisplayName = "VecFloat CrossProduct", CompactNodeTitle = "^"))
    static FVectorFloat ProjectOnTo(const FVectorFloat& A, const FVectorFloat& B);

};
