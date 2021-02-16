// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "Core/Public/CoreTypes.h"
#include "Core/Public/Containers/UnrealString.h"
#include "Core/Public/Misc/AutomationTest.h"

#include "SpaceKitPrecision/Public/RotatorFloat.h"
#include "SpaceKitPrecision/Public/QuatFloat.h"


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCoreMathRotatorFloatOperatorsTest, "SpaceKitPrecision.RotatorFloatMath.Operators", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

#pragma optimize("", off)

bool FCoreMathRotatorFloatOperatorsTest::RunTest(const FString& Parameters)
{
	// Base operators work
	FRotatorFloat x(6_fl, 12_fl, 20_fl);
	FRotatorFloat y(2_fl, 3_fl, 4_fl);
	FRealFloat z(2.0);

	TestEqual(TEXT("Predefined elem 1"), x.Yaw.ToFloat(), 6.f);
	TestEqual(TEXT("Predefined elem 2"), x.Pitch.ToFloat(), 12.f);
	TestEqual(TEXT("Predefined elem 3"), x.Roll.ToFloat(), 20.f);

	// Base operators: addition, substraction, multiplication, division
	TestEqual(TEXT("Predefined addition 1"), x + y, FRotatorFloat(8_fl, 15_fl, 24_fl));
	TestEqual(TEXT("Predefined addition 2"), x + (-y), FRotatorFloat(4_fl, 9_fl, 16_fl));
	TestEqual(TEXT("Predefined substraction 1"), x - y, FRotatorFloat(4_fl, 9_fl, 16_fl));
	TestEqual(TEXT("Predefined substraction 2"), x - (-y), FRotatorFloat(8_fl, 15_fl, 24_fl));
	TestEqual(TEXT("Predefined multiplication 1"), x * y, FRotatorFloat(12_fl, 36_fl, 80_fl));
	TestEqual(TEXT("Predefined multiplication 2"), x * (-y), FRotatorFloat(FRealFloat(-12.0), FRealFloat(-36.0), FRealFloat(-80.0)));
	TestEqual(TEXT("Predefined multiplication 3"), x * z, FRotatorFloat(12_fl, 24_fl, 40_fl));
	TestEqual(TEXT("Predefined division 1"), x / y, FRotatorFloat(3_fl, 4_fl, 5_fl));
	TestEqual(TEXT("Predefined division 2"), x / (-y), FRotatorFloat(FRealFloat(-3.0), FRealFloat(-4.0), FRealFloat(-5.0)));
	TestEqual(TEXT("Predefined division 3"), x / z, FRotatorFloat(3_fl, 6_fl, 10_fl));

	// Add assign
	{
		FRotatorFloat a = x;
		a += y;
		TestEqual(TEXT("Predefined add assign vec operator works"), a, FRotatorFloat(8_fl, 15_fl, 24_fl));
	}

	// Sub assign
	{
		FRotatorFloat a = x;
		a -= y;
		TestEqual(TEXT("Predefined sub assign vec operator works"), a, FRotatorFloat(4_fl, 9_fl, 16_fl));
	}

	// Mult assign vec
	{
		FRotatorFloat a = x;
		a *= y;
		TestEqual(TEXT("Predefined mult assign vec operator works"), a, FRotatorFloat(12_fl, 36_fl, 80_fl));
	}

	// Mult assign real
	{
		FRotatorFloat a = x;
		a *= z;
		TestEqual(TEXT("Predefined mult assign real operator works"), a, FRotatorFloat(12_fl, 24_fl, 40_fl));
	}

	// Div assign vec
	{
		FRotatorFloat a = x;
		a /= y;
		TestEqual(TEXT("Predefined div assign vec operator works"), a, FRotatorFloat(3_fl, 4_fl, 5_fl));
	}

	// Div assign real
	{
		FRotatorFloat a = x;
		a /= z;
		TestEqual(TEXT("Predefined div assign real operator works"), a, FRotatorFloat(3_fl, 6_fl, 10_fl));
	}

	TestEqual(TEXT("Predefined axis 1"), x.GetAxis(EAxis::Z).ToFloat(), 6.f);
	TestEqual(TEXT("Predefined axis 2"), x.GetAxis(EAxis::Y).ToFloat(), 12.f);
	TestEqual(TEXT("Predefined axis 3"), x.GetAxis(EAxis::X).ToFloat(), 20.f);

	const FVectorFloat Vec1(1_fl, 0.2_fl, 5_fl);
	
	TestEqual(TEXT("Predefined rotation 1"), FRotatorFloat(180_fl, 0_fl, 0_fl).RotateVector(Vec1).ToFVector(), FVector(-1.f, -0.2f, 5.f));
	TestEqual(TEXT("Predefined rotation 2"), FRotatorFloat(90_fl, 0_fl, 0_fl).RotateVector(Vec1).ToFVector(), FVector(-0.2f, 1.f, 5.f));

	return true;
}

#pragma optimize("", on)

#endif //WITH_DEV_AUTOMATION_TESTS
