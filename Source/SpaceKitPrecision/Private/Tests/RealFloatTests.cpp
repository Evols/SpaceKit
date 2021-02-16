// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "Core/Public/CoreTypes.h"
#include "Core/Public/Containers/UnrealString.h"
#include "Core/Public/Misc/AutomationTest.h"

#include "SpaceKitPrecision/Public/RealFloat.h"


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCoreMathFloatOperatorsTest, "SpaceKitPrecision.FloatingPointMath.Operators", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

#pragma optimize("", off)

bool FCoreMathFloatOperatorsTest::RunTest(const FString& Parameters)
{
	// Base operators work
	FRealFloat x("6");
	FRealFloat y("2");

	// Base operators: addition, substraction, multiplication, division
	TestEqual(TEXT("Predefined addition 1"), x + y, 8_fl);
	TestEqual(TEXT("Predefined addition 2"), x + (-y), 4_fl);
	TestEqual(TEXT("Predefined substraction 1"), x - y, 4_fl);
	TestEqual(TEXT("Predefined substraction 2"), x - (-y), 8_fl);
	TestEqual(TEXT("Predefined multiplication 1"), x * y, 12_fl);
	TestEqual(TEXT("Predefined multiplication 2"), x * (-y), -12_fl);
	TestEqual(TEXT("Predefined division 1"), x / y, 3_fl);
	TestEqual(TEXT("Predefined division 2"), x / (-y), -3_fl);

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
		FRealFloat a("7");
		FRealFloat b = a;
		b = 3_fl;

		TestEqual(TEXT("Predefined copy operator works"), b, 3_fl);
		TestTrue(TEXT("Predefined copy operator doesn't keep reference 1"), a != b);
		TestEqual(TEXT("Predefined copy operator doesn't keep reference 2"), a, 7_fl);
	}

	// Add assign
	{
		FRealFloat a("7");
		a += 3_fl;
		TestEqual(TEXT("Predefined add assign operator works"), a, 10_fl);
	}
	
	// Sub assign
	{
		FRealFloat a("7");
		a -= 3_fl;
		TestEqual(TEXT("Predefined sub assign operator works 1"), a, 4_fl);
		a -= 5_fl;
		TestEqual(TEXT("Predefined add assign operator works 2"), a, -1_fl);
	}

	// Mult assign
	{
		FRealFloat a("7");
		a *= 3_fl;
		TestEqual(TEXT("Predefined mult assign operator works 1"), a, 21_fl);
		a *= -1_fl;
		TestEqual(TEXT("Predefined mult assign operator works 2"), a, -21_fl);
	}

	// Div assign
	{
		FRealFloat a("6");
		a /= 2_fl;
		TestEqual(TEXT("Predefined div assign operator works 1"), a, 3_fl);
		a /= -12_fl;
		TestEqual(TEXT("Predefined div assign operator works 2"), a, -0.25_fl);
	}

	return true;
}

#pragma optimize("", on)


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpacePrecisionFloatAdvancedMath, "SpaceKitPrecision.FloatingPointMath.AdvancedMath", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

#pragma optimize("", off)

bool FSpacePrecisionFloatAdvancedMath::RunTest(const FString& Parameters)
{
	// Test real to string
	{
		const FRealFloat a("2");
		const FString str = URealFloatMath::ConvRealToString(a);
		TestEqual(TEXT("Predefined conversion to string"), str, "2");
	}

	// Test string to real
	{
		TestEqual(TEXT("Predefined conversion from string 1"), URealFloatMath::ConvStringToReal("2"), 2_fl);
		TestEqual(TEXT("Predefined conversion from string 2"), URealFloatMath::ConvStringToReal("2.4"), 2.4_fl);
		TestEqual(TEXT("Predefined conversion from string 3"), URealFloatMath::ConvStringToReal("1000000000"), 1000000000_fl);
		TestEqual(TEXT("Predefined conversion from string 4"), URealFloatMath::ConvStringToReal("-102"), -102_fl);
	}

	// Test string bidirectional conversions
	{
		TestEqual(TEXT("Predefined conversion string bidirectional 1"), URealFloatMath::ConvStringToReal(URealFloatMath::ConvRealToString(2_fl)), 2_fl);
		TestEqual(TEXT("Predefined conversion string bidirectional 2"), URealFloatMath::ConvStringToReal(URealFloatMath::ConvRealToString(2.4_fl)), 2.4_fl);
		TestEqual(TEXT("Predefined conversion string bidirectional 3"), URealFloatMath::ConvStringToReal(URealFloatMath::ConvRealToString(1000000000_fl)), 1000000000_fl);
		TestEqual(TEXT("Predefined conversion string bidirectional 4"), URealFloatMath::ConvStringToReal(URealFloatMath::ConvRealToString(-102_fl)), -102_fl);
	}

	// Test real to float
	{
		const FRealFloat a("2.5");
		const float b = URealFloatMath::ConvRealToFloat(a);
		TestEqual(TEXT("Predefined conversion to float"), b, 2.5f);
	}

	// Test float to real
	{
		TestEqual(TEXT("Predefined conversion from float 1"), URealFloatMath::ConvFloatToReal(2.f), 2_fl);
		TestEqual(TEXT("Predefined conversion from float 2"), URealFloatMath::ConvFloatToReal(2.5f), 2.5_fl);
		TestEqual(TEXT("Predefined conversion from float 3"), URealFloatMath::ConvFloatToReal(100000.f), 100000_fl);
		TestEqual(TEXT("Predefined conversion from float 4"), URealFloatMath::ConvFloatToReal(-102.f), -102_fl);
	}

	// Test float bidirectional conversions
	{
		TestEqual(TEXT("Predefined conversion float bidirectional 1"), URealFloatMath::ConvFloatToReal(URealFloatMath::ConvRealToFloat(2_fl)), 2_fl);
		TestEqual(TEXT("Predefined conversion float bidirectional 2"), URealFloatMath::ConvFloatToReal(URealFloatMath::ConvRealToFloat(2.5_fl)), 2.5_fl);
		TestEqual(TEXT("Predefined conversion float bidirectional 3"), URealFloatMath::ConvFloatToReal(URealFloatMath::ConvRealToFloat(100000_fl)), 100000_fl);
		TestEqual(TEXT("Predefined conversion float bidirectional 4"), URealFloatMath::ConvFloatToReal(URealFloatMath::ConvRealToFloat(-102_fl)), -102_fl);
	}

	// Normalize angle rad
	{
		TestEqual(TEXT("Predefined normalize angle rad 1"), URealFloatMath::NormalizeAngleRad(FRealFloat::Pi * 0.24_fl).ToFloat(), (FRealFloat::Pi * 0.24_fl).ToFloat());
		TestEqual(TEXT("Predefined normalize angle rad 2"), URealFloatMath::NormalizeAngleRad(FRealFloat::Pi * 2.84_fl).ToFloat(), (FRealFloat::Pi * 0.84_fl).ToFloat());
		TestEqual(TEXT("Predefined normalize angle rad 3"), URealFloatMath::NormalizeAngleRad(FRealFloat::Pi * 1.43_fl).ToFloat(), (FRealFloat::Pi * -0.57_fl).ToFloat());
		TestEqual(TEXT("Predefined normalize angle rad 4"), URealFloatMath::NormalizeAngleRad(FRealFloat::Pi * 20.24_fl).ToFloat(), (FRealFloat::Pi * 0.24_fl).ToFloat());
		TestEqual(TEXT("Predefined normalize angle rad 5"), URealFloatMath::NormalizeAngleRad(FRealFloat::Pi * -0.24_fl).ToFloat(), (FRealFloat::Pi * -0.24_fl).ToFloat());
		TestEqual(TEXT("Predefined normalize angle rad 6"), URealFloatMath::NormalizeAngleRad(FRealFloat::Pi * -1.65_fl).ToFloat(), (FRealFloat::Pi * 0.35_fl).ToFloat());
	}

	// Cos rad
	{
		TestEqual(TEXT("Predefined cosrad 1"), URealFloatMath::CosRad(0_fl).ToFloat(), 1_fl.ToFloat());
		TestEqual(TEXT("Predefined cosrad 2"), URealFloatMath::CosRad(FRealFloat::Pi * 0.5_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined cosrad 3"), URealFloatMath::CosRad(FRealFloat::Pi).ToFloat(), FRealFloat(-1.0).ToFloat());
		TestEqual(TEXT("Predefined cosrad 4"), URealFloatMath::CosRad(FRealFloat::Pi * 20_fl).ToFloat(), 1_fl.ToFloat());
		TestEqual(TEXT("Predefined cosrad 5"), URealFloatMath::CosRad(FRealFloat::Pi * 0.25_fl).ToFloat(), (URealFloatMath::Sqrt(2_fl) / 2_fl).ToFloat());
	}

	// Sin rad35
	{
		TestEqual(TEXT("Predefined sinrad 1"), URealFloatMath::SinRad(0_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined sinrad 2"), URealFloatMath::SinRad(FRealFloat::Pi).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined sinrad 3"), URealFloatMath::SinRad(FRealFloat::Pi * 20_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined sinrad 4"), URealFloatMath::SinRad(FRealFloat::Pi * 0.5_fl).ToFloat(), 1_fl.ToFloat());
		TestEqual(TEXT("Predefined sinrad 5"), URealFloatMath::SinRad(FRealFloat::Pi * 0.25_fl).ToFloat(), (URealFloatMath::Sqrt(2_fl) / 2_fl).ToFloat());
	}

	// Tan rad
	{
		TestEqual(TEXT("Predefined tanrad 1"), URealFloatMath::TanRad(0_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined tanrad 2"), URealFloatMath::TanRad(FRealFloat::Pi * 0.25_fl).ToFloat(), 1_fl.ToFloat());
		TestEqual(TEXT("Predefined tanrad 3"), URealFloatMath::TanRad(-FRealFloat::Pi * 0.25_fl).ToFloat(), FRealFloat(-1.0).ToFloat());
		TestEqual(TEXT("Predefined tanrad 4"), URealFloatMath::TanRad(FRealFloat::Pi).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined tanrad 5"), URealFloatMath::TanRad(-FRealFloat::Pi).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined tanrad 6"), URealFloatMath::TanRad(20_fl * FRealFloat::Pi).ToFloat(), 0_fl.ToFloat());
	}

	// Normalize angle deg
	{
		TestEqual(TEXT("Predefined normalize angle deg 1"), URealFloatMath::NormalizeAngleDeg(48_fl), 48_fl);
		TestEqual(TEXT("Predefined normalize angle deg 2"), URealFloatMath::NormalizeAngleDeg(875_fl), 155_fl);
		TestEqual(TEXT("Predefined normalize angle deg 3"), URealFloatMath::NormalizeAngleDeg(287_fl), -73_fl);
		TestEqual(TEXT("Predefined normalize angle deg 4"), URealFloatMath::NormalizeAngleDeg(-524_fl), -164_fl);
		TestEqual(TEXT("Predefined normalize angle deg 5"), URealFloatMath::NormalizeAngleDeg(-194_fl), 166_fl);
	}

	// Cos deg
	{
		TestEqual(TEXT("Predefined cosdeg 1"), URealFloatMath::CosDeg(0_fl).ToFloat(), 1_fl.ToFloat());
		TestEqual(TEXT("Predefined cosdeg 2"), URealFloatMath::CosDeg(90_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined cosdeg 3"), URealFloatMath::CosDeg(180_fl).ToFloat(), FRealFloat(-1.0).ToFloat());
		TestEqual(TEXT("Predefined cosdeg 4"), URealFloatMath::CosDeg(7200_fl).ToFloat(), 1_fl.ToFloat());
		TestEqual(TEXT("Predefined cosdeg 5"), URealFloatMath::CosDeg(45_fl).ToFloat(), (URealFloatMath::Sqrt(2_fl) / 2_fl).ToFloat());
	}

	// Sin deg
	{
		TestEqual(TEXT("Predefined sindeg 1"), URealFloatMath::SinDeg(0_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined sindeg 2"), URealFloatMath::SinDeg(180_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined sindeg 3"), URealFloatMath::SinDeg(7200_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined sindeg 4"), URealFloatMath::SinDeg(90_fl).ToFloat(), 1_fl.ToFloat());
		TestEqual(TEXT("Predefined sindeg 5"), URealFloatMath::SinDeg(45_fl).ToFloat(), (URealFloatMath::Sqrt(2_fl) / 2_fl).ToFloat());
	}

	// Tan deg
	{
		TestEqual(TEXT("Predefined tandeg 1"), URealFloatMath::TanDeg(0_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined tandeg 2"), URealFloatMath::TanDeg(45_fl).ToFloat(), 1_fl.ToFloat());
		TestEqual(TEXT("Predefined tandeg 3"), URealFloatMath::TanDeg(FRealFloat(-45.0)).ToFloat(), FRealFloat(-1.0).ToFloat());
		TestEqual(TEXT("Predefined tandeg 4"), URealFloatMath::TanDeg(180_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined tandeg 5"), URealFloatMath::TanDeg(FRealFloat(-180.0)).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined tandeg 6"), URealFloatMath::TanDeg(7200_fl).ToFloat(), 0_fl.ToFloat());
	}

	// Acos rad
	{
		TestEqual(TEXT("Predefined acosrad 1"), URealFloatMath::AcosRad(URealFloatMath::CosRad(0_fl)).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined acosrad 2"), URealFloatMath::AcosRad(URealFloatMath::CosRad(FRealFloat::Pi * 0.5_fl)).ToFloat(), (FRealFloat::Pi * 0.5_fl).ToFloat());
		TestEqual(TEXT("Predefined acosrad 3"), URealFloatMath::AcosRad(URealFloatMath::CosRad(FRealFloat::Pi * 1.5_fl)).ToFloat(), (FRealFloat::Pi * 0.5_fl).ToFloat());
		TestEqual(TEXT("Predefined acosrad 4"), URealFloatMath::AcosRad(URealFloatMath::CosRad(-FRealFloat::Pi * 0.5_fl)).ToFloat(), (FRealFloat::Pi * 0.5_fl).ToFloat());
		TestEqual(TEXT("Predefined acosrad 5"), URealFloatMath::AcosRad(URealFloatMath::CosRad(1_fl)).ToFloat(), 1_fl.ToFloat());
	}

	// Asin rad
	{
		TestEqual(TEXT("Predefined asinrad 1"), URealFloatMath::AsinRad(URealFloatMath::SinRad(0_fl)).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined asinrad 2"), URealFloatMath::AsinRad(URealFloatMath::SinRad(FRealFloat::Pi * 0.5_fl)).ToFloat(), (FRealFloat::Pi * 0.5_fl).ToFloat());
		TestEqual(TEXT("Predefined asinrad 3"), URealFloatMath::AsinRad(URealFloatMath::SinRad(FRealFloat::Pi * 1.5_fl)).ToFloat(), (FRealFloat::Pi * FRealFloat(-0.5)).ToFloat());
		TestEqual(TEXT("Predefined asinrad 4"), URealFloatMath::AsinRad(URealFloatMath::SinRad(-FRealFloat::Pi * 0.5_fl)).ToFloat(), (FRealFloat::Pi * FRealFloat(-0.5)).ToFloat());
		TestEqual(TEXT("Predefined asinrad 5"), URealFloatMath::AsinRad(URealFloatMath::SinRad(1_fl)).ToFloat(), 1_fl.ToFloat());
	}

	// Atan rad
	{
		TestEqual(TEXT("Predefined atanrad 1"), URealFloatMath::AtanRad(URealFloatMath::TanRad(0_fl)).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined atanrad 2"), URealFloatMath::AtanRad(URealFloatMath::TanRad(FRealFloat::Pi * 0.5_fl)).ToFloat(), (FRealFloat::Pi * 0.5_fl).ToFloat());
		TestEqual(TEXT("Predefined atanrad 3"), URealFloatMath::AtanRad(URealFloatMath::TanRad(1_fl)).ToFloat(), 1_fl.ToFloat());
	}

	// Atan2 rad
	{
		TestEqual(TEXT("Predefined atan2rad 1"), URealFloatMath::Atan2Rad(0_fl, 1_fl).ToFloat(), (FRealFloat::Pi * 0_fl).ToFloat());
		TestEqual(TEXT("Predefined atan2rad 2"), URealFloatMath::Atan2Rad(1_fl, 0_fl).ToFloat(), (FRealFloat::Pi * 0.5_fl).ToFloat());
		TestEqual(TEXT("Predefined atan2rad 3"), URealFloatMath::Atan2Rad(0_fl, FRealFloat(-1.0)).ToFloat(), (FRealFloat::Pi * FRealFloat(-1.0)).ToFloat());
		TestEqual(TEXT("Predefined atan2rad 4"), URealFloatMath::Atan2Rad(FRealFloat(-1.0), 0_fl).ToFloat(), (FRealFloat::Pi * FRealFloat(-0.5)).ToFloat());
		TestEqual(TEXT("Predefined atan2rad 5"), URealFloatMath::Atan2Rad(URealFloatMath::Sqrt(3_fl), 1_fl).ToFloat(), (FRealFloat::Pi * FRealFloat(1.0 / 3.0)).ToFloat());
		TestEqual(TEXT("Predefined atan2rad 6"), URealFloatMath::Atan2Rad(URealFloatMath::Sqrt(3_fl), FRealFloat(-1.0)).ToFloat(), (FRealFloat::Pi * FRealFloat(2.0 / 3.0)).ToFloat());
		TestEqual(TEXT("Predefined atan2rad 7"), URealFloatMath::Atan2Rad(-URealFloatMath::Sqrt(3_fl), 1_fl).ToFloat(), (FRealFloat::Pi * FRealFloat(-1.0 / 3.0)).ToFloat());
		TestEqual(TEXT("Predefined atan2rad 8"), URealFloatMath::Atan2Rad(-URealFloatMath::Sqrt(3_fl), FRealFloat(-1.0)).ToFloat(), (FRealFloat::Pi * FRealFloat(-2.0 / 3.0)).ToFloat());
	}


	// Acos deg
	{
		TestEqual(TEXT("Predefined acosdeg 1"), URealFloatMath::AcosDeg(URealFloatMath::CosDeg(0_fl)).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined acosdeg 2"), URealFloatMath::AcosDeg(URealFloatMath::CosDeg(90_fl)).ToFloat(), 90_fl.ToFloat());
		TestEqual(TEXT("Predefined acosdeg 3"), URealFloatMath::AcosDeg(URealFloatMath::CosDeg(270_fl)).ToFloat(), 90_fl.ToFloat());
		TestEqual(TEXT("Predefined acosdeg 4"), URealFloatMath::AcosDeg(URealFloatMath::CosDeg(FRealFloat(-90.0))).ToFloat(), 90_fl.ToFloat());
		TestEqual(TEXT("Predefined acosdeg 5"), URealFloatMath::AcosDeg(URealFloatMath::CosDeg(87.6_fl)).ToFloat(), 87.6_fl.ToFloat());
	}

	// Asin deg
	{
		TestEqual(TEXT("Predefined asindeg 1"), URealFloatMath::AsinDeg(URealFloatMath::SinDeg(0_fl)).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined asindeg 2"), URealFloatMath::AsinDeg(URealFloatMath::SinDeg(90_fl)).ToFloat(), (90_fl).ToFloat());
		TestEqual(TEXT("Predefined asindeg 3"), URealFloatMath::AsinDeg(URealFloatMath::SinDeg(270_fl)).ToFloat(), FRealFloat(-90.0).ToFloat());
		TestEqual(TEXT("Predefined asindeg 4"), URealFloatMath::AsinDeg(URealFloatMath::SinDeg(-90_fl)).ToFloat(), FRealFloat(-90.0).ToFloat());
		TestEqual(TEXT("Predefined asindeg 5"), URealFloatMath::AsinDeg(URealFloatMath::SinDeg(87.6_fl)).ToFloat(), 87.6_fl.ToFloat());
	}

	// Atan deg
	{
		TestEqual(TEXT("Predefined atandeg 1"), URealFloatMath::AtanDeg(URealFloatMath::TanDeg(0_fl)).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined atandeg 2"), URealFloatMath::AtanDeg(URealFloatMath::TanDeg(90_fl)).ToFloat(), (90_fl).ToFloat());
		TestEqual(TEXT("Predefined atandeg 3"), URealFloatMath::AtanDeg(URealFloatMath::TanDeg(1_fl)).ToFloat(), 1_fl.ToFloat());
	}

	// Atan2 deg
	{
		TestEqual(TEXT("Predefined atan2deg 1"), URealFloatMath::Atan2Deg(0_fl, 1_fl).ToFloat(), 0_fl.ToFloat());
		TestEqual(TEXT("Predefined atan2deg 2"), URealFloatMath::Atan2Deg(1_fl, 0_fl).ToFloat(), 90_fl.ToFloat());
		TestEqual(TEXT("Predefined atan2deg 3"), URealFloatMath::Atan2Deg(0_fl, FRealFloat(-1.0)).ToFloat(), FRealFloat(-180.0).ToFloat());
		TestEqual(TEXT("Predefined atan2deg 4"), URealFloatMath::Atan2Deg(FRealFloat(-1.0), 0_fl).ToFloat(), FRealFloat(-90.0).ToFloat());
		TestEqual(TEXT("Predefined atan2deg 5"), URealFloatMath::Atan2Deg(URealFloatMath::Sqrt(3_fl), 1_fl).ToFloat(), 60_fl.ToFloat());
		TestEqual(TEXT("Predefined atan2deg 6"), URealFloatMath::Atan2Deg(URealFloatMath::Sqrt(3_fl), FRealFloat(-1.0)).ToFloat(), 120_fl.ToFloat());
		TestEqual(TEXT("Predefined atan2deg 7"), URealFloatMath::Atan2Deg(-URealFloatMath::Sqrt(3_fl), 1_fl).ToFloat(), FRealFloat(-60.0).ToFloat());
		TestEqual(TEXT("Predefined atan2deg 8"), URealFloatMath::Atan2Deg(-URealFloatMath::Sqrt(3_fl), FRealFloat(-1.0)).ToFloat(), FRealFloat(-120.0).ToFloat());
	}

	// Pow
	{
		TestEqual(TEXT("Predefined pow 1"), URealFloatMath::Pow(2_fl, 3_fl).ToFloat(), 8.f);
		TestEqual(TEXT("Predefined pow 2"), URealFloatMath::Pow(2_fl, FRealFloat(-1.0)).ToFloat(), 0.5f);
		TestEqual(TEXT("Predefined pow 3"), URealFloatMath::Pow(4_fl, 0.5_fl).ToFloat(), 2.f);
		TestEqual(TEXT("Predefined pow 4"), URealFloatMath::Pow(4_fl, 0_fl).ToFloat(), 1.f);
	}

	// Sqrt
	{
		TestEqual(TEXT("Predefined sqrt 1"), URealFloatMath::Sqrt(4_fl).ToFloat(), 2_fl.ToFloat());
		TestEqual(TEXT("Predefined sqrt 2"), URealFloatMath::Sqrt(1_fl).ToFloat(), 1_fl.ToFloat());
		TestEqual(TEXT("Predefined sqrt 3"), URealFloatMath::Sqrt(0.25_fl).ToFloat(), 0.5_fl.ToFloat());
	}

	// Exp
	{
		TestEqual(TEXT("Predefined exp 1"), URealFloatMath::Exp(1_fl).ToFloat(), expf(1.f));
		TestEqual(TEXT("Predefined exp 2"), URealFloatMath::Exp(5_fl).ToFloat(), expf(5.f));
		TestEqual(TEXT("Predefined exp 3"), URealFloatMath::Exp(0.1_fl).ToFloat(), expf(0.1f));
		TestEqual(TEXT("Predefined exp 4"), URealFloatMath::Exp(0_fl).ToFloat(), 1.f);
		TestEqual(TEXT("Predefined exp 5"), URealFloatMath::Exp(FRealFloat(-5.0)).ToFloat(), expf(-5.f));
	}

	// LogE
	{
		TestEqual(TEXT("Predefined loge 1"), URealFloatMath::LogE(1_fl).ToFloat(), logf(1.f));
		TestEqual(TEXT("Predefined loge 2"), URealFloatMath::LogE(5_fl).ToFloat(), logf(5.f));
		TestEqual(TEXT("Predefined loge 3"), URealFloatMath::LogE(0.1_fl).ToFloat(), logf(0.1f));
		TestEqual(TEXT("Predefined loge 4"), URealFloatMath::LogE(FRealFloat(-5.0)).ToFloat(), logf(-5.f));
		TestEqual(TEXT("Predefined loge 5"), URealFloatMath::Exp(URealFloatMath::LogE(0.658_fl)).ToFloat(), 0.658f);
		TestEqual(TEXT("Predefined loge 6"), URealFloatMath::Exp(URealFloatMath::LogE(6_fl)).ToFloat(), 6.f);
	}

	// Log2
	{
		TestEqual(TEXT("Predefined log2 1"), URealFloatMath::Log2(1_fl).ToFloat(), 0.f);
		TestEqual(TEXT("Predefined log2 2"), URealFloatMath::Log2(256_fl).ToFloat(), 8.f);
		TestEqual(TEXT("Predefined log2 3"), URealFloatMath::Log2(0.125_fl).ToFloat(), -3.f);
		TestEqual(TEXT("Predefined log2 4"), URealFloatMath::Log2(6.2_fl).ToFloat(), log2f(6.2f));
		TestEqual(TEXT("Predefined log2 5"), URealFloatMath::Log2(0.298_fl).ToFloat(), log2f(0.298f));
	}

	// Log10
	{
		TestEqual(TEXT("Predefined log10 1"), URealFloatMath::Log10(1_fl).ToFloat(), 0.f);
		TestEqual(TEXT("Predefined log10 2"), URealFloatMath::Log10(1000_fl).ToFloat(), 3.f);
		TestEqual(TEXT("Predefined log10 3"), URealFloatMath::Log10(0.01_fl).ToFloat(), -2.f);
		TestEqual(TEXT("Predefined log10 4"), URealFloatMath::Log10(6.2_fl).ToFloat(), log10f(6.2f));
		TestEqual(TEXT("Predefined log10 5"), URealFloatMath::Log10(0.298_fl).ToFloat(), log10f(0.298f));
	}

	// Min
	{
		TestEqual(TEXT("Predefined min 1"), URealFloatMath::Min(1_fl, 2_fl).ToFloat(), 1.f);
		TestEqual(TEXT("Predefined min 2"), URealFloatMath::Min(2_fl, 1_fl).ToFloat(), 1.f);
		TestEqual(TEXT("Predefined min 3"), URealFloatMath::Min(FRealFloat(-2.0), 1_fl).ToFloat(), -2.f);
		TestEqual(TEXT("Predefined min 4"), URealFloatMath::Min(FRealFloat(-2.0), FRealFloat(-5.0)).ToFloat(), -5.f);
	}

	// Max
	{
		TestEqual(TEXT("Predefined max 1"), URealFloatMath::Max(1_fl, 2_fl).ToFloat(), 2.f);
		TestEqual(TEXT("Predefined max 2"), URealFloatMath::Max(2_fl, 1_fl).ToFloat(), 2.f);
		TestEqual(TEXT("Predefined max 3"), URealFloatMath::Max(FRealFloat(-2.0), 1_fl).ToFloat(), 1.f);
		TestEqual(TEXT("Predefined max 4"), URealFloatMath::Max(FRealFloat(-2.0), FRealFloat(-5.0)).ToFloat(), -2.f);
	}

	// Clamp
	{
		TestEqual(TEXT("Predefined clamp 1"), URealFloatMath::Clamp(1_fl, 2_fl, 3_fl).ToFloat(), 2.f);
		TestEqual(TEXT("Predefined clamp 2"), URealFloatMath::Clamp(4_fl, 2_fl, 3_fl).ToFloat(), 3.f);
		TestEqual(TEXT("Predefined clamp 3"), URealFloatMath::Clamp(2_fl, 2_fl, 3_fl).ToFloat(), 2.f);
		TestEqual(TEXT("Predefined clamp 4"), URealFloatMath::Clamp(3_fl, 2_fl, 3_fl).ToFloat(), 3.f);
		TestEqual(TEXT("Predefined clamp 5"), URealFloatMath::Clamp(2.5_fl, 2_fl, 3_fl).ToFloat(), 2.5f);
	}

	// Abs
	{
		TestEqual(TEXT("Predefined abs 1"), URealFloatMath::Abs(1_fl).ToFloat(), 1.f);
		TestEqual(TEXT("Predefined abs 2"), URealFloatMath::Abs(FRealFloat(-1.0)).ToFloat(), 1.f);
		TestEqual(TEXT("Predefined abs 3"), URealFloatMath::Abs(0_fl).ToFloat(), 0.f);
	}

	// Sign
	{
		TestEqual(TEXT("Predefined sign 1"), URealFloatMath::Sign(2_fl).ToFloat(), 1.f);
		TestEqual(TEXT("Predefined sign 2"), URealFloatMath::Sign(FRealFloat(-2.0)).ToFloat(), -1.f);
		TestEqual(TEXT("Predefined sign 3"), URealFloatMath::Sign(0_fl).ToFloat(), 0.f);
	}

	return true;
}

#pragma optimize("", on)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpacePrecisionFloatLimitsTest, "SpaceKitPrecision.FloatingPointMath.Limits", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

#pragma optimize("", off)

bool FSpacePrecisionFloatLimitsTest::RunTest(const FString& Parameters)
{
	const FRealFloat Factor = 2_fl;

	// Find the maximum factor difference between the min and max value
	FRealFloat MaxDeltaTempNext = 2_fl, MaxDeltaTemp = 1_fl;

	while (MaxDeltaTempNext + 1_fl != MaxDeltaTempNext)
	{
		MaxDeltaTemp = MaxDeltaTempNext;
		MaxDeltaTempNext *= Factor;
	}

	TestTrue(TEXT("Max delta is over 10^38"), MaxDeltaTemp >= 1e38_fl);

	return true;
}

#pragma optimize("", on)

#endif //WITH_DEV_AUTOMATION_TESTS
