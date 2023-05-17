// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "SpaceKitPrecision/Public/VectorFixed.h"
#include "SpaceKitPrecision/Public/VectorFloat.h"

FVectorFixed FVectorFixed::Identity = FVectorFixed();

FVectorFixed UVectorFixedMath::ConvFVectorToVectorFixed(const FVector& InVec)
{
	return FVectorFixed(InVec);
}

FVector UVectorFixedMath::ConvVectorFixedToFVector(const FVectorFixed& InVec)
{
	return InVec.ToFVector();
}

FVectorFixed UVectorFixedMath::MakeVectorFixed(FRealFixed X, FRealFixed Y, FRealFixed Z)
{
	return FVectorFixed(X, Y, Z);
}

void UVectorFixedMath::BreakVectorFixed(const FVectorFixed& Vec, FRealFixed& X, FRealFixed& Y, FRealFixed& Z)
{
	X = Vec.X;
	Y = Vec.Y;
	Z = Vec.Z;
}

FVectorFixed UVectorFixedMath::VecPlusVec(const FVectorFixed& First, const FVectorFixed& Second)
{
	return First + Second;
}

FVectorFixed UVectorFixedMath::VecMinusVec(const FVectorFixed& First, const FVectorFixed& Second)
{
	return First - Second;
}

FVectorFixed UVectorFixedMath::VecMultReal(const FVectorFixed& First, const FRealFixed& Second)
{
	return First * Second;
}

FVectorFixed UVectorFixedMath::VecMultVec(const FVectorFixed& First, const FVectorFixed& Second)
{
	return First * Second;
}

FVectorFixed UVectorFixedMath::VecDivReal(const FVectorFixed& First, const FRealFixed& Second)
{
	return First / Second;
}

FVectorFixed UVectorFixedMath::VecDivVec(const FVectorFixed& First, const FVectorFixed& Second)
{
	return First / Second;
}

bool UVectorFixedMath::VecEqualsVec(const FVectorFixed& First, const FVectorFixed& Second, const FRealFixed& Tolerance)
{
	return First.Equals(Second, Tolerance);
}

bool UVectorFixedMath::VecNotEqualsVec(const FVectorFixed& First, const FVectorFixed& Second, const FRealFixed& Tolerance)
{
	return !UVectorFixedMath::VecEqualsVec(First, Second, Tolerance);
}

FRealFixed UVectorFixedMath::DotProduct(const FVectorFixed& First, const FVectorFixed& Second)
{
	return First | Second;
}

FVectorFixed UVectorFixedMath::CrossProduct(const FVectorFixed& First, const FVectorFixed& Second)
{
	return First ^ Second;
}

FVectorFixed UVectorFixedMath::ProjectOnTo(const FVectorFixed& B, const FVectorFixed& A)
{
	return (B * ((A | B) / (B | B)));
}
