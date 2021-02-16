// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "Core/Public/CoreTypes.h"
#include "Core/Public/Containers/UnrealString.h"
#include "Core/Public/Misc/AutomationTest.h"

#include "SpaceKitPrecision/Public/RealFloat.h"
#include "SpaceKitPrecision/Public/VectorFloat.h"
#include "SpaceKitPrecision/Public/QuatFloat.h"


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCoreMathQuatFloatOperatorsTest, "SpaceKitPrecision.QuatFloatMath.Operators", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

#pragma optimize("", off)

bool FCoreMathQuatFloatOperatorsTest::RunTest(const FString& Parameters)
{
	const FVectorFloat Vec1(1_fl, 0.2_fl, 5_fl);
	const FQuatFloat Quat(FVectorFloat(0_fl, 0_fl, 1_fl), 90_fl);

	TestEqual(TEXT("Predefined rotation 1"), Quat.RotateVector(Vec1).ToFVector(), FVector(-0.2f, 1.f, 5.f));

	return true;
}

#pragma optimize("", on)

#endif //WITH_DEV_AUTOMATION_TESTS
