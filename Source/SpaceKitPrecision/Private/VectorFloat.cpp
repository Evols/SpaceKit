// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "SpaceKitPrecision/Public/VectorFloat.h"
#include "SpaceKitPrecision/Public/VectorFixed.h"

FVectorFloat FVectorFloat::Identity = FVectorFloat();

FVectorFloat UVectorFloatMath::ConvFVectorToVectorFloat(const FVector& InVec)
{
    return FVectorFloat(InVec);
}

FVector UVectorFloatMath::ConvVectorFloatToFVector(const FVectorFloat& InVec)
{
    return InVec.ToFVector();
}

FVectorFloat UVectorFloatMath::MakeVectorFloat(const FRealFloat& X, const FRealFloat& Y, const FRealFloat& Z)
{
	return FVectorFloat(X, Y, Z);
}

void UVectorFloatMath::BreakVectorFloat(const FVectorFloat& Vec, FRealFloat& X, FRealFloat& Y, FRealFloat& Z)
{
	X = Vec.X;
	Y = Vec.Y;
	Z = Vec.Z;
}

FVectorFloat UVectorFloatMath::VecPlusVec(const FVectorFloat& First, const FVectorFloat& Second)
{
	return First + Second;
}

FVectorFloat UVectorFloatMath::VecMinusVec(const FVectorFloat& First, const FVectorFloat& Second)
{
	return First - Second;
}

FVectorFloat UVectorFloatMath::VecMultReal(const FVectorFloat& First, const FRealFloat& Second)
{
	return First * Second;
}

FVectorFloat UVectorFloatMath::VecMultVec(const FVectorFloat& First, const FVectorFloat& Second)
{
	return First * Second;
}

FVectorFloat UVectorFloatMath::VecDivReal(const FVectorFloat& First, const FRealFloat& Second)
{
	return First / Second;
}

FVectorFloat UVectorFloatMath::VecDivVec(const FVectorFloat& First, const FVectorFloat& Second)
{
	return First / Second;
}

bool UVectorFloatMath::VecEqualsVec(const FVectorFloat& First, const FVectorFloat& Second, const FRealFloat& Tolerance)
{
	return First.Equals(Second, Tolerance);
}

bool UVectorFloatMath::VecNotEqualsVec(const FVectorFloat& First, const FVectorFloat& Second, const FRealFloat& Tolerance)
{
	return !UVectorFloatMath::VecEqualsVec(First, Second, Tolerance);
}

FRealFloat UVectorFloatMath::DotProduct(const FVectorFloat& First, const FVectorFloat& Second)
{
	return First | Second;
}

FVectorFloat UVectorFloatMath::CrossProduct(const FVectorFloat& First, const FVectorFloat& Second)
{
	return First ^ Second;
}

FVectorFloat UVectorFloatMath::ProjectOnTo(const FVectorFloat& B, const FVectorFloat& A)
{
	return (B * ((A | B) / (B | B)));
}
