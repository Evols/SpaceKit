// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "Core/Public/CoreTypes.h"
#include "Core/Public/Containers/UnrealString.h"
#include "Core/Public/Misc/AutomationTest.h"

#include "SpaceKitPrecision/Public/VectorFixed.h"


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCoreMathVectorFixedOperatorsTest, "SpaceKitPrecision.VectorFixedMath.Operators", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

#pragma optimize("", off)

bool FCoreMathVectorFixedOperatorsTest::RunTest(const FString& Parameters)
{
	// Base operators work
	FVectorFixed x(6_fx, 12_fx, 20_fx);
	FVectorFixed y(2_fx, 3_fx, 4_fx);
	FRealFixed z = 2_fx;

	TestEqual(TEXT("Predefined elem 1"), x.X.ToFloat(), 6.f);
	TestEqual(TEXT("Predefined elem 2"), x.Y.ToFloat(), 12.f);
	TestEqual(TEXT("Predefined elem 3"), x.Z.ToFloat(), 20.f);

	// Base operators: addition, substraction, multiplication, division
	TestEqual(TEXT("Predefined addition 1"), x + y, FVectorFixed(8_fx, 15_fx, 24_fx));
	TestEqual(TEXT("Predefined addition 2"), x + (-y), FVectorFixed(4_fx, 9_fx, 16_fx));
	TestEqual(TEXT("Predefined substraction 1"), x - y, FVectorFixed(4_fx, 9_fx, 16_fx));
	TestEqual(TEXT("Predefined substraction 2"), x - (-y), FVectorFixed(8_fx, 15_fx, 24_fx));
	TestEqual(TEXT("Predefined multiplication 1"), x * y, FVectorFixed(12_fx, 36_fx, 80_fx));
	TestEqual(TEXT("Predefined multiplication 2"), x * (-y), FVectorFixed(-12_fx, -36_fx, -80_fx));
	TestEqual(TEXT("Predefined multiplication 3"), x * z, FVectorFixed(12_fx, 24_fx, 40_fx));
	TestEqual(TEXT("Predefined division 1"), x / y, FVectorFixed(3_fx, 4_fx, 5_fx));
	TestEqual(TEXT("Predefined division 2"), x / (-y), FVectorFixed(-3_fx, -4_fx, -5_fx));
	TestEqual(TEXT("Predefined division 3"), x / z, FVectorFixed(3_fx, 6_fx, 10_fx));

	// Add assign
	{
		FVectorFixed a = x;
		a += y;
		TestEqual(TEXT("Predefined add assign vec operator works"), a, FVectorFixed(8_fx, 15_fx, 24_fx));
	}

	// Sub assign
	{
		FVectorFixed a = x;
		a -= y;
		TestEqual(TEXT("Predefined sub assign vec operator works"), a, FVectorFixed(4_fx, 9_fx, 16_fx));
	}

	// Mult assign vec
	{
		FVectorFixed a = x;
		a *= y;
		TestEqual(TEXT("Predefined mult assign vec operator works"), a, FVectorFixed(12_fx, 36_fx, 80_fx));
	}

	// Mult assign real
	{
		FVectorFixed a = x;
		a *= z;
		TestEqual(TEXT("Predefined mult assign real operator works"), a, FVectorFixed(12_fx, 24_fx, 40_fx));
	}

	// Div assign vec
	{
		FVectorFixed a = x;
		a /= y;
		TestEqual(TEXT("Predefined div assign vec operator works"), a, FVectorFixed(3_fx, 4_fx, 5_fx));
	}

	// Div assign real
	{
		FVectorFixed a = x;
		a /= z;
		TestEqual(TEXT("Predefined div assign real operator works"), a, FVectorFixed(3_fx, 6_fx, 10_fx));
	}

	TestEqual(TEXT("Predefined dot 1"), (FVectorFixed(4_fx, 5_fx, 12_fx) | FVectorFixed(16_fx, 4_fx, -7_fx)).ToFloat(), 0.f);
	TestEqual(TEXT("Predefined dot 2"), (FVectorFixed(8_fx, -2_fx, 7_fx) | FVectorFixed(11_fx, 6_fx, -3_fx)).ToFloat(), 55.f);

	{
		const FVectorFixed Cross = x ^ y;
		TestEqual(TEXT("Predefined cross 1"), Cross, FVectorFixed(-12_fx, 16_fx, -6_fx));
		TestEqual(TEXT("Predefined cross 2"), (Cross | x).ToFloat(), 0.f);
		TestEqual(TEXT("Predefined cross 3"), (Cross | y).ToFloat(), 0.f);
	}

	TestEqual(TEXT("Predefined size 1"), FVectorFixed(3_fx, 4_fx, 12_fx).Size().ToFloat(), 13.f);
	TestEqual(TEXT("Predefined size squared 1"), (FVectorFixed(3_fx, 4_fx, 12_fx).Size() * FVectorFixed(3_fx, 4_fx, 12_fx).Size()).ToFloat(), FVectorFixed(3_fx, 4_fx, 12_fx).SizeSquared().ToFloat());

	{
		const FVectorFixed Normal = x.GetNormal();
		TestEqual(TEXT("Predefined normal 1"), Normal.Size().ToFloat(), 1.f);
		TestEqual(TEXT("Predefined normal 2"), (Normal | x).ToFloat(), x.Size().ToFloat());
		TestEqual(TEXT("Predefined normal 3"), (Normal ^ x).Size().ToFloat(), 0.f);
	}

	TestEqual(TEXT("Predefined axis 1"), x.GetAxis(EAxis::X).ToFloat(), 6.f);
	TestEqual(TEXT("Predefined axis 2"), x.GetAxis(EAxis::Y).ToFloat(), 12.f);
	TestEqual(TEXT("Predefined axis 3"), x.GetAxis(EAxis::Z).ToFloat(), 20.f);

	return true;
}

#pragma optimize("", on)

#endif //WITH_DEV_AUTOMATION_TESTS
