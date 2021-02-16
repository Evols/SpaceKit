// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "Core/Public/CoreTypes.h"
#include "Core/Public/Containers/UnrealString.h"
#include "Core/Public/Misc/AutomationTest.h"

#include "SpaceKitPrecision/Public/VectorFloat.h"


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCoreMathVectorFloatOperatorsTest, "SpaceKitPrecision.VectorFloatMath.Operators", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

#pragma optimize("", off)

bool FCoreMathVectorFloatOperatorsTest::RunTest(const FString& Parameters)
{
	// Base operators work
	FVectorFloat x(6_fl, 12_fl, 20_fl);
	FVectorFloat y(2_fl, 3_fl, 4_fl);
	FRealFloat z(2.0);
	
	TestEqual(TEXT("Predefined elem 1"), x.X.ToFloat(), 6.f);
	TestEqual(TEXT("Predefined elem 2"), x.Y.ToFloat(), 12.f);
	TestEqual(TEXT("Predefined elem 3"), x.Z.ToFloat(), 20.f);

	// Base operators: addition, substraction, multiplication, division
	TestEqual(TEXT("Predefined addition 1"), x + y, FVectorFloat(8_fl, 15_fl, 24_fl));
	TestEqual(TEXT("Predefined addition 2"), x + (-y), FVectorFloat(4_fl, 9_fl, 16_fl));
	TestEqual(TEXT("Predefined substraction 1"), x - y, FVectorFloat(4_fl, 9_fl, 16_fl));
	TestEqual(TEXT("Predefined substraction 2"), x - (-y), FVectorFloat(8_fl, 15_fl, 24_fl));
	TestEqual(TEXT("Predefined multiplication 1"), x * y, FVectorFloat(12_fl, 36_fl, 80_fl));
	TestEqual(TEXT("Predefined multiplication 2"), x * (-y), FVectorFloat(FRealFloat(-12.0), FRealFloat(-36.0), FRealFloat(-80.0)));
	TestEqual(TEXT("Predefined multiplication 3"), x * z, FVectorFloat(12_fl, 24_fl, 40_fl));
	TestEqual(TEXT("Predefined division 1"), x / y, FVectorFloat(3_fl, 4_fl, 5_fl));
	TestEqual(TEXT("Predefined division 2"), x / (-y), FVectorFloat(FRealFloat(-3.0), FRealFloat(-4.0), FRealFloat(-5.0)));
	TestEqual(TEXT("Predefined division 3"), x / z, FVectorFloat(3_fl, 6_fl, 10_fl));

	// Add assign
	{
		FVectorFloat a = x;
		a += y;
		TestEqual(TEXT("Predefined add assign vec operator works"), a, FVectorFloat(8_fl, 15_fl, 24_fl));
	}

	// Sub assign
	{
		FVectorFloat a = x;
		a -= y;
		TestEqual(TEXT("Predefined sub assign vec operator works"), a, FVectorFloat(4_fl, 9_fl, 16_fl));
	}

	// Mult assign vec
	{
		FVectorFloat a = x;
		a *= y;
		TestEqual(TEXT("Predefined mult assign vec operator works"), a, FVectorFloat(12_fl, 36_fl, 80_fl));
	}

	// Mult assign real
	{
		FVectorFloat a = x;
		a *= z;
		TestEqual(TEXT("Predefined mult assign real operator works"), a, FVectorFloat(12_fl, 24_fl, 40_fl));
	}

	// Div assign vec
	{
		FVectorFloat a = x;
		a /= y;
		TestEqual(TEXT("Predefined div assign vec operator works"), a, FVectorFloat(3_fl, 4_fl, 5_fl));
	}

	// Div assign real
	{
		FVectorFloat a = x;
		a /= z;
		TestEqual(TEXT("Predefined div assign real operator works"), a, FVectorFloat(3_fl, 6_fl, 10_fl));
	}

	TestEqual(TEXT("Predefined dot 1"), (FVectorFloat(4_fl, 5_fl, 12_fl) | FVectorFloat(16_fl, 4_fl, FRealFloat(-7.0))).ToFloat(), 0.f);
	TestEqual(TEXT("Predefined dot 2"), (FVectorFloat(8_fl, FRealFloat(-2.0), 7_fl) | FVectorFloat(11_fl, 6_fl, FRealFloat(-3.0))).ToFloat(), 55.f);

	{
		const FVectorFloat Cross = x ^ y;
		TestEqual(TEXT("Predefined cross 1"), Cross, FVectorFloat(FRealFloat(-12.0), 16_fl, FRealFloat(-6.0)));
		TestEqual(TEXT("Predefined cross 2"), (Cross | x).ToFloat(), 0.f);
		TestEqual(TEXT("Predefined cross 3"), (Cross | y).ToFloat(), 0.f);
	}

	TestEqual(TEXT("Predefined size 1"), FVectorFloat(3_fl, 4_fl, 12_fl).Size().ToFloat(), 13.f);
	TestEqual(TEXT("Predefined size squared 1"), URealFloatMath::Pow(FVectorFloat(3_fl, 4_fl, 12_fl).Size(), 2_fl).ToFloat(), FVectorFloat(3_fl, 4_fl, 12_fl).SizeSquared().ToFloat());

	{
		const FVectorFloat Normal = x.GetNormal();
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
