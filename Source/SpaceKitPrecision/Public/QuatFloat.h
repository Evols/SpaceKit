// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "SpaceKitPrecision/Public/VectorFloat.h"

#include "QuatFloat.generated.h"

struct FRotatorFloat;

// Similar to an FQuat, but using reals instead of floats
USTRUCT(BlueprintType)
struct SPACEKITPRECISION_API FQuatFloat
{
    GENERATED_BODY()

public:

	UPROPERTY()
    FRealFloat X;

	UPROPERTY()
    FRealFloat Y;
    
	UPROPERTY()
    FRealFloat Z;

	UPROPERTY()
    FRealFloat W;
	
    static FQuatFloat Identity;

	// Constructor for an identity quaternion
    FQuatFloat()
        : X("0.0"), Y("0.0"), Z("0.0"), W("1.0")
    {
    }

	// Constructor with individual axes (X, y, z, W). To create a quaternion from axis and angle, see MakeFromAxes()
    FQuatFloat(const FRealFloat& nX, const FRealFloat& nY, const FRealFloat& nZ, const FRealFloat& nW)
        : X(nX), Y(nY), Z(nZ), W(nW)
    {
    }

    // Builds a quaternion from a rotation axis, and the angle to rotate. Axis is expected to be normalized!
    FQuatFloat(const FVectorFloat& Axis, FRealFloat AngleDeg)
    {
        const FRealFloat HalfAngle = 0.5_fl * AngleDeg;
        const FRealFloat Cos = URealFloatMath::CosDeg(HalfAngle);
        const FRealFloat Sin = URealFloatMath::SinDeg(HalfAngle);

        X = Sin * Axis.X;
        Y = Sin * Axis.Y;
        Z = Sin * Axis.Z;
        W = Cos;
    }

    // Builds a quaternion from an FQuat
    explicit FQuatFloat(const FQuat& Quat)
        : X(Quat.X), Y(Quat.Y), Z(Quat.Z), W(Quat.W)
    {
    }
    
    // Builds a quaternion from a rotator
    explicit FQuatFloat(const FRotatorFloat& Rotator);

    // Rotates a given vector by this quaternion
    FVectorFloat RotateVector(const FVectorFloat& Vec) const
    {
        const FVectorFloat Q(X, Y, Z);
        const FVectorFloat T = FVectorFloat::CrossProduct(Q, Vec) * 2_fl;
        return Vec + (T * W) + FVectorFloat::CrossProduct(Q, T);
    }

	// Rotates backward a given vector by this quaternion, so that for a given quaternion Q and a given vector V, UnrotateVector(RotateVector(V)) = V
    FVectorFloat UnrotateVector(const FVectorFloat& Vec) const
    {
        const FVectorFloat Q(-X, -Y, -Z);
        const FVectorFloat T = FVectorFloat::CrossProduct(Q, Vec) * 2_fl;
        return Vec + (T * W) + FVectorFloat::CrossProduct(Q, T);
    }

	// Normalizes this quaternion. Note that you can only apply a rotation to a vector using a normalized quaternion. Not normalized quaternion's results are undefined
    FQuatFloat GetNormalized()
    {
        const FRealFloat Size = URealFloatMath::Sqrt(X * X + Y * Y + Z * Z + W * W);
        return FQuatFloat(X / Size, Y / Size, Z / Size, W / Size);
    }

    // Combines two quaternions, so that for given quaternions Q1 and Q2, and QR = Q1*Q2, QR's rotation corresponds to applying the rotation of Q1, then the rotation of Q2
    FQuatFloat operator*(const FQuatFloat& Other)
    {
        const FRealFloat T0 = (Z - Y) * (Other.Y - Other.Z);
        const FRealFloat T1 = (W + X) * (Other.W + Other.X);
        const FRealFloat T2 = (W - X) * (Other.Y + Other.Z);
        const FRealFloat T3 = (Y + Z) * (Other.W - Other.X);
        const FRealFloat T4 = (Z - X) * (Other.X - Other.Y);
        const FRealFloat T5 = (Z + X) * (Other.X + Other.Y);
        const FRealFloat T6 = (W + Y) * (Other.W - Other.Z);
        const FRealFloat T7 = (W - Y) * (Other.W + Other.Z);
        const FRealFloat T8 = T5 + T6 + T7;
        const FRealFloat T9 = 0.5_fl * (T4 + T8);

        return FQuatFloat(T1 + T9 - T8, T2 + T9 - T7, T3 + T9 - T6, T0 + T9 - T5);
    }

	// Create a quaternion from a coordinate system, using two vectors (the third will be computed).
	// The axes these vectors represent are given by Axes. The only supported values are XY, YZ and XZ
    static FQuatFloat MakeFromAxes(const FVectorFloat& Vec1, const FVectorFloat& Vec2, const EAxisList::Type Axes = EAxisList::XY)
    {
        const FVectorFloat NewVec1 = Vec1.GetNormal();
        const FVectorFloat NewVec3 = (NewVec1 ^ Vec2).GetNormal();
        const FVectorFloat NewVec2 = (NewVec3 ^ NewVec1).GetNormal();

        const FVectorFloat& VecX = (Axes & EAxisList::X) ? NewVec1 : NewVec3;
        const FVectorFloat& VecY = Axes == EAxisList::XY ? NewVec2 : Axes == EAxisList::YZ ? NewVec1 : NewVec3;
        const FVectorFloat& VecZ = (Axes & EAxisList::Z) ? NewVec2 : NewVec3;

        const FRealFloat M[3][3] = {
        	{ VecX.X, VecX.Y, VecX.Z },
        	{ VecY.X, VecY.Y, VecY.Z },
        	{ VecZ.X, VecZ.Y, VecZ.Z }
        };

        const int32 i = VecY.Y > VecX.X ? 1 : VecZ.Z > VecY.Y ? 2 : 0;

        const int32 nxt[3] = { 1, 2, 0 };
        const int32 j = nxt[i];
        const int32 k = nxt[j];

        const FRealFloat InvS = 1_fl / URealFloatMath::Sqrt(M[i][i] - M[j][j] - M[k][k] + 1_fl);
        const FRealFloat s = 0.5_fl * InvS;

        FRealFloat qt[4];
        qt[i] = 0.5_fl / InvS;
        qt[3] = (M[j][k] - M[k][j]) * s;
        qt[j] = (M[i][j] + M[j][i]) * s;
        qt[k] = (M[i][k] + M[k][i]) * s;

        return FQuatFloat(qt[0], qt[1], qt[2], qt[3]);
    }

	// Converts this to a string, with almost 100% precision, except for very small numbers (see FRealFloat.ToString())
    FString ToString() const
    {
        return FString::Printf(TEXT("(X=%s,Y=%s,Z=%s,W=%s)"), *X.ToString(), *Y.ToString(), *Z.ToString(), *W.ToString());
    }

    // Converts this to an FQuat
    FQuat ToFQuat() const
    {
        return FQuat(X.ToFloat(), Y.ToFloat(), Z.ToFloat(), W.ToFloat());
    }
	
    // Gets the axis and angle
    void ToAxisAndAngle(FVectorFloat& OutAxis, FRealFloat& OutAngle) const
    {
        const FRealFloat Size = URealFloatMath::Sqrt(FMath::Max(1_fl - (W * W), 0_fl));

        if (Size >= FRealFloat(1e-14))
        {
            OutAxis = FVectorFloat(X / Size, Y / Size, Z / Size);
        }
        else
        {
            OutAxis = FVectorFloat(1.0, 0.0, 0.0);
        }

        OutAngle = 2_fl * URealFloatMath::AcosDeg(W);
    }

    FQuatFloat Inverse() const
    {
        return FQuatFloat(-X, -Y, -Z, W);
    }
};

struct FRotatorFloat;

/**
 * Blueprints math library for QuatFloat
 */
UCLASS(BlueprintType, Abstract)
class SPACEKITPRECISION_API UQuatFloatMath : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
	
// Basic conversions
public:

    UFUNCTION(BlueprintPure, category = "QuatFloat", meta = (DisplayName = "FQuat to QuatFloat", CompactNodeTitle = "->", BlueprintAutocast))
    static FQuatFloat ConvFQuatToQuatFloat(const FQuat& Quat);

    UFUNCTION(BlueprintPure, category = "QuatFloat", meta = (DisplayName = "QuatFloat to FQuat", CompactNodeTitle = "->", BlueprintAutocast))
    static FQuat ConvQuatFloatToFQuat(const FQuatFloat& Quat);
    
    UFUNCTION(BlueprintPure, category = "QuatFloat", meta = (DisplayName = "Make QuatFloat", CompactNodeTitle = "Make"))
    static FQuatFloat MakeQuatFloat(const FVectorFloat& Axis, const FRealFloat& Angle);

    UFUNCTION(BlueprintPure, category = "QuatFloat", meta = (DisplayName = "Break QuatFloat", CompactNodeTitle = "Make"))
    static void BreakQuatFloat(const FQuatFloat& Quat, FVectorFloat& Axis, FRealFloat& Angle);

// Basic QuatFloat math
public:
    
    UFUNCTION(BlueprintPure, category = "QuatFloat", meta=(DisplayName = "Inv QuatFloat", CompactNodeTitle="Inv"))
    static FQuatFloat InvQuat(const FQuatFloat& Quat);

    UFUNCTION(BlueprintPure, category = "QuatFloat", meta = (DisplayName = "QuatFloat == QuatFloat", CompactNodeTitle = "=="))
    static bool QuatEqualsQuat(const FQuatFloat& First, const FQuatFloat& Second, const FRealFloat& Tolerance);
    
	UFUNCTION(BlueprintPure, category = "QuatFloat", meta = (DisplayName = "QuatFloat != QuatFloat", CompactNodeTitle = "!="))
    static bool QuatNotEqualsQuat(const FQuatFloat& First, const FQuatFloat& Second, const FRealFloat& Tolerance);

	UFUNCTION(BlueprintPure, category = "QuatFloat", meta = (DisplayName = "QuatFloat != QuatFloat", CompactNodeTitle = "!="))
    static FQuatFloat Slerp(FQuatFloat First, FQuatFloat Second, const FRealFloat& Alpha);

};
