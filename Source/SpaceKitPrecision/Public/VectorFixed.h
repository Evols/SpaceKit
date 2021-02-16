// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "SpaceKitPrecision/Public/RealFixed.h"
#include "VectorFloat.h"

#include "VectorFixed.generated.h"


/*
 * Similar to an FVector, but using fixed-point reals instead of floats.
 */
USTRUCT(BlueprintType)
struct SPACEKITPRECISION_API FVectorFixed
{
    GENERATED_BODY()

public:

    UPROPERTY()
    FRealFixed X;

    UPROPERTY()
    FRealFixed Y;

    UPROPERTY()
    FRealFixed Z;

    static FVectorFixed Identity;

    FVectorFixed()
        : X(0.0), Y(0.0), Z(0.0)
    {
    }

    FVectorFixed(FRealFixed InX, FRealFixed InY, FRealFixed InZ)
        : X(InX), Y(InY), Z(InZ)
    {
    }

    explicit FVectorFixed(const FVector& InVec)
        : X(InVec.X), Y(InVec.Y), Z(InVec.Z)
    {
    }

    // Vector math
public:
	
    FVectorFixed operator+(const FVectorFixed& Other) const
    {
        return FVectorFixed(X + Other.X, Y + Other.Y, Z + Other.Z);
    }

    FVectorFixed& operator+=(const FVectorFixed& Other)
    {
        return *this = *this + Other;
    }

    FVectorFixed operator-(const FVectorFixed& Other) const
    {
        return FVectorFixed(X - Other.X, Y - Other.Y, Z - Other.Z);
    }

    FVectorFixed& operator-=(const FVectorFixed& Other)
    {
        return *this = *this - Other;
    }

    FVectorFixed operator*(const FVectorFixed& Other) const
    {
        return FVectorFixed(X * Other.X, Y * Other.Y, Z * Other.Z);
    }

    FVectorFixed& operator*=(const FVectorFixed& Other)
    {
        return *this = *this * Other;
    }

    FVectorFixed operator*(FRealFixed Other) const
    {
        return FVectorFixed(X * Other, Y * Other, Z * Other);
    }

    FVectorFixed& operator*=(FRealFixed Other)
    {
        return *this = *this * Other;
    }

    FVectorFixed operator/(const FVectorFixed& Other) const
    {
        return FVectorFixed(X / Other.X, Y / Other.Y, Z / Other.Z);
    }

    FVectorFixed& operator/=(const FVectorFixed& Other)
    {
        return *this = *this / Other;
    }

    FVectorFixed operator/(FRealFixed Other) const
    {
        return FVectorFixed(X / Other, Y / Other, Z / Other);
    }

    FVectorFixed& operator/=(FRealFixed Other)
    {
        return *this = *this / Other;
    }

    FVectorFixed operator-() const
    {
        return FVectorFixed(-X, -Y, -Z);
    }

    FVector ToFVector() const
    {
        return FVector(X.ToFloat(), Y.ToFloat(), Z.ToFloat());
    }

    static FRealFixed DotProduct(const FVectorFixed& Vec, const FVectorFixed& Other)
    {
        return Vec.X * Other.X + Vec.Y * Other.Y + Vec.Z * Other.Z;
    }

    FRealFixed operator|(const FVectorFixed& Other) const
    {
        return DotProduct(*this, Other);
    }

    static FVectorFixed CrossProduct(const FVectorFixed& Vec, const FVectorFixed& Other)
    {
        return FVectorFixed(Vec.Y * Other.Z - Vec.Z * Other.Y, Vec.Z * Other.X - Vec.X * Other.Z, Vec.X * Other.Y - Vec.Y * Other.X);
    }

    FVectorFixed operator^(const FVectorFixed& Other) const
    {
        return CrossProduct(*this, Other);
    }

    bool Equals(const FVectorFixed& Other, FRealFixed Tolerance = FRealFixed(1.0e-8)) const
    {
        return (*this - Other).GetAbsSum() <= Tolerance;
    }

    bool operator==(const FVectorFixed& Other) const
    {
        return Equals(Other);
    }

    bool operator!=(const FVectorFixed& Other) const
    {
        return ! (*this == Other);
    }

    FRealFixed SizeSquared() const
    {
        return DotProduct(*this, *this);
    }

    FRealFixed Size() const
    {
        const auto SizeSq = SizeSquared();
        return URealFixedMath::Sqrt(SizeSq);
    }

    FVectorFixed GetNormal(FRealFixed Tolerance = FRealFixed(1.0e-8)) const
    {
        const FRealFixed ThisSize = Size();
        if (ThisSize < Tolerance) return Identity;
        return *this / ThisSize;
    }

    FRealFixed GetAbsSum() const
    {
        return URealFixedMath::Abs(X) + URealFixedMath::Abs(Y) + URealFixedMath::Abs(Z);
    }

    FString ToString() const
    {
        return FString::Printf(TEXT("(X=%s,Y=%s,Z=%s)"), *X.ToString(), *Y.ToString(), *Z.ToString());
    }

    FRealFixed& GetAxis(EAxis::Type Axis)
    {
        return Axis == EAxis::X ? X : Axis == EAxis::Y ? Y : Z;
    }

    const FRealFixed& GetAxis(EAxis::Type Axis) const
    {
        return Axis == EAxis::X ? X : Axis == EAxis::Y ? Y : Z;
    }
};

/**
 * Blueprints math library for VectorFixed
 */
UCLASS(BlueprintType, Abstract)
class SPACEKITPRECISION_API UVectorFixedMath : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
	
// Basic conversions
public:

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "FVector to VectorFixed", CompactNodeTitle = "->", BlueprintAutocast))
    static FVectorFixed ConvFVectorToVectorFixed(const FVector& InVec);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "VectorFixed to FVector", CompactNodeTitle = "->", BlueprintAutocast))
    static FVector ConvVectorFixedToFVector(const FVectorFixed& InVec);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "Make VectorFixed", CompactNodeTitle = "Make"))
    static FVectorFixed MakeVectorFixed(const FRealFixed& X, const FRealFixed& Y, const FRealFixed& Z);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "Break VectorFixed", CompactNodeTitle = "Make"))
    static void BreakVectorFixed(const FVectorFixed& Vec, FRealFixed& X, FRealFixed& Y, FRealFixed& Z);

// Basic VecFixed math
public:

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta=(DisplayName = "VecFixed + VecFixed", CompactNodeTitle="+"))
    static FVectorFixed VecPlusVec(const FVectorFixed& First, const FVectorFixed& Second);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta=(DisplayName = "VecFixed - VecFixed", CompactNodeTitle="-"))
    static FVectorFixed VecMinusVec(const FVectorFixed& First, const FVectorFixed& Second);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta=(DisplayName = "VecFixed * RealFixed", CompactNodeTitle="*"))
    static FVectorFixed VecMultReal(const FVectorFixed& First, const FRealFixed& Second);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "VecFixed * VecFixed", CompactNodeTitle = "*"))
    static FVectorFixed VecMultVec(const FVectorFixed& First, const FVectorFixed& Second);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta=(DisplayName = "VecFixed / RealFixed", CompactNodeTitle="/"))
    static FVectorFixed VecDivReal(const FVectorFixed& First, const FRealFixed& Second);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "VecFixed / VecFixed", CompactNodeTitle = "/"))
    static FVectorFixed VecDivVec(const FVectorFixed& First, const FVectorFixed& Second);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "VecFixed == VecFixed", CompactNodeTitle = "=="))
    static bool VecEqualsVec(const FVectorFixed& First, const FVectorFixed& Second, const FRealFixed& Tolerance);

	UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "VecFixed != VecFixed", CompactNodeTitle = "!="))
    static bool VecNotEqualsVec(const FVectorFixed& First, const FVectorFixed& Second, const FRealFixed& Tolerance);

// Advanced RealFixed math
public:
    
    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "VecFixed DotProduct", CompactNodeTitle = "|"))
    static FRealFixed DotProduct(const FVectorFixed& First, const FVectorFixed& Second);
    
    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "VecFixed CrossProduct", CompactNodeTitle = "^"))
    static FVectorFixed CrossProduct(const FVectorFixed& First, const FVectorFixed& Second);

    UFUNCTION(BlueprintPure, category = "VectorFixed", meta = (DisplayName = "VecFixed CrossProduct", CompactNodeTitle = "^"))
    static FVectorFixed ProjectOnTo(const FVectorFixed& A, const FVectorFixed& B);

};
