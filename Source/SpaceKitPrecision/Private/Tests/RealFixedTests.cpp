// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "Core/Public/CoreTypes.h"
#include "Core/Public/Containers/UnrealString.h"
#include "Core/Public/Misc/AutomationTest.h"

#include "SpaceKitPrecision/Public/RealFixed.h"


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCoreMathFixedOperatorsTest, "SpaceKitPrecision.FixedPointMath.Operators", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

#pragma optimize("", off)

bool FCoreMathFixedOperatorsTest::RunTest(const FString& Parameters)
{
	// Base operators work
	FRealFixed x = 6_fx;
	FRealFixed y = 2_fx;

	// Base operators: addition, substraction, multiplication, division
	TestEqual(TEXT("Predefined addition 1"), x + y, 8_fx);
	TestEqual(TEXT("Predefined addition 2"), x + (-y), 4_fx);
	TestEqual(TEXT("Predefined substraction 1"), x - y, 4_fx);
	TestEqual(TEXT("Predefined substraction 2"), x - (-y), 8_fx);
	TestEqual(TEXT("Predefined multiplication 1"), x * y, 12_fx);
	TestEqual(TEXT("Predefined multiplication 2"), x * (-y), -12_fx);
	TestEqual(TEXT("Predefined division 1"), x / y, 3_fx);
	TestEqual(TEXT("Predefined division 2"), x / (-y), -3_fx);
	// Modulo: use ToFloat to have a tolerance
	TestEqual(TEXT("Predefined modulo, with float tolerance"), (6.2_fx % 1.4_fx).ToFloat(), 0.6f);

	// Equality/inequality operators

	// Strictly inferior
	TestTrue(TEXT("Predefined strictly inferior with actually inferior values"), y < x);
	TestFalse(TEXT("Predefined strictly inferior with equal values"), x < x);
	TestFalse(TEXT("Predefined strictly inferior with superior values"), x < y);

	// Strictly superior
	TestTrue(TEXT("Predefined strictly superior with actually inferior values"), x > y);
	TestFalse(TEXT("Predefined strictly superior with equal values"), x > x);
	TestFalse(TEXT("Predefined strictly superior with superior values"), y > x);

	// Inferior or equal
	TestTrue(TEXT("Predefined inferior or equal with actually inferior values"), y <= x);
	TestTrue(TEXT("Predefined inferior or equal with actually equal values"), x <= x);
	TestFalse(TEXT("Predefined inferior or equal with superior values"), x <= y);

	// Superior or equal
	TestTrue(TEXT("Predefined superior or equal with actually inferior values"), x >= y);
	TestTrue(TEXT("Predefined superior or equal with actually equal values"), x >= x);
	TestFalse(TEXT("Predefined superior or equal with superior values"), y >= x);

	// Equal
	TestTrue(TEXT("Predefined equal with actually equal values"), x == x);
	TestFalse(TEXT("Predefined superior or equal with different values"), y == x);

	// Different 
	TestTrue(TEXT("Predefined different with actually different values"), x != y);
	TestFalse(TEXT("Predefined different with equals values"), x != x);

	// Ensure the copy operator doesn't keep reference
	{
		FRealFixed a = 7_fx;
		FRealFixed b = a;
		b = 3_fx;

		TestEqual(TEXT("Predefined copy operator works"), b, 3_fx);
		TestTrue(TEXT("Predefined copy operator doesn't keep reference 1"), a != b);
		TestEqual(TEXT("Predefined copy operator doesn't keep reference 2"), a, 7_fx);
	}

	// Add assign
	{
		FRealFixed a = 7_fx;
		a += 3_fx;
		TestEqual(TEXT("Predefined add assign operator works"), a, 10_fx);
	}
	
	// Sub assign
	{
		FRealFixed a = 7_fx;
		a -= 3_fx;
		TestEqual(TEXT("Predefined sub assign operator works 1"), a, 4_fx);
		a -= 5_fx;
		TestEqual(TEXT("Predefined add assign operator works 2"), a, -1_fx);
	}

	// Mult assign
	{
		FRealFixed a = 7_fx;
		a *= 3_fx;
		TestEqual(TEXT("Predefined mult assign operator works 1"), a, 21_fx);
		a *= -1_fx;
		TestEqual(TEXT("Predefined mult assign operator works 2"), a, -21_fx);
	}

	// Div assign
	{
		FRealFixed a = 6_fx;
		a /= 2_fx;
		TestEqual(TEXT("Predefined div assign operator works 1"), a, 3_fx);
		a /= -12_fx;
		TestEqual(TEXT("Predefined div assign operator works 2"), a, -0.25_fx);
	}

	// Mod assign
	{
		FRealFixed a = 8_fx;
		a %= 3_fx;
		TestEqual(TEXT("Predefined mod assign operator works 1"), a, 2_fx);
		a %= 0.3_fx;
		TestEqual(TEXT("Predefined mod assign operator works 2"), a.ToFloat(), 0.2_fx.ToFloat());
	}

	return true;
}

#pragma optimize("", on)


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCoreMathFixedLimitsTest, "SpaceKitPrecision.FixedPointMath.Limits", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

#pragma optimize("", off)

bool FCoreMathFixedLimitsTest::RunTest(const FString& Parameters)
{
	const FRealFixed LyToCm = 946073047258004200_fx;
	const FRealFixed NmToCm = 1e-7_fx;
	const FRealFixed Factor = 2_fx;

	const FRealFixed ExpectedUpperLimit = 35000_fx * LyToCm; // 35000 lightyears
	const FRealFixed ExpectedLowerLimit = 0.2_fx * NmToCm; // 0.2nm. The order matters here

	// Find upper limit
	FRealFixed UpperLimitTempNext = 2_fx, UpperLimitTemp = 1_fx;

	while (UpperLimitTempNext / Factor == UpperLimitTemp)
	{
		UpperLimitTemp = UpperLimitTempNext;
		UpperLimitTempNext *= Factor;
	}

	const FRealFixed ComputedUpperLimit = ((UpperLimitTemp - 1_fx) * 2_fx + 1_fx);
	
	TestTrue(TEXT("Max value is over 35000 lightyears"), ComputedUpperLimit >= ExpectedUpperLimit);

	// Find lower limit
	FRealFixed LowerLimitTempNext = 1_fx, LowerLimitTemp = 2_fx;

	while (LowerLimitTempNext * Factor == LowerLimitTemp)
	{
		LowerLimitTemp = LowerLimitTempNext;
		LowerLimitTempNext /= Factor;
	}

	const FRealFixed ComputedLowerLimit = LowerLimitTemp;

	TestTrue(TEXT("Min value is under 0.2 nanometers"), ComputedLowerLimit <= ExpectedLowerLimit);

	// Ensure stability of operations
	const FRealFixed Ul = ExpectedUpperLimit;
	const FRealFixed UlPLl = Ul + ExpectedLowerLimit;
	TestEqual(TEXT("Stability is kept, as adding a very small value (0.2nm) to a very big one (35000ly) is measurably kept"), UlPLl - ExpectedUpperLimit, ExpectedLowerLimit);

	return true;
}

#pragma optimize("", on)


#endif //WITH_DEV_AUTOMATION_TESTS
