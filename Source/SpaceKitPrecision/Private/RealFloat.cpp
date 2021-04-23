
// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "SpaceKitPrecision/Public/RealFloat.h"

#include "SpaceKitPrecision/SpaceKitPrecision.h"
#include "SpaceKitPrecision/Public/RealFixed.h"


FRealFloat::FRealFloat()
{
    Value = 0.0;
}

FRealFloat::FRealFloat(const FRealFloat& InValue)
{
    Value = InValue.Value;
}

FRealFloat::FRealFloat(const ttBigType& InValue)
{
    Value = InValue;
}

FRealFloat::FRealFloat(int32 InValue)
{
    Value = InValue;
}

FRealFloat::FRealFloat(uint32 InValue)
{
    Value = InValue;
}

FRealFloat::FRealFloat(int64 InValue)
{
    Value = InValue;
}

FRealFloat::FRealFloat(uint64 InValue)
{
    Value = InValue;
}

FRealFloat::FRealFloat(float InValue)
{
    Value = InValue;
}

FRealFloat::FRealFloat(double InValue)
{
    Value = InValue;
}

FRealFloat::FRealFloat(const char* InValue)
{
    Value = ttBigType(InValue);
}

FRealFloat::FRealFloat(const std::string& InValue)
{
	if (IsFloat(FString(InValue.c_str())))
	{
	    Value = ttBigType(InValue.c_str());
    }
}

FRealFloat::FRealFloat(const FString& InValue)
{
	if (IsFloat(InValue))
	{
		Value = ttBigType(TCHAR_TO_ANSI(*InValue));
    }
}

static FRealFloat::ttBigType GenPi()
{
    return FRealFloat::ttBigType("3.141592653589793238462643383279502884197");
}

static FRealFloat::ttBigType GenHalfPi()
{
    return GenPi() * FRealFloat::ttBigType(0.5);
}

FRealFloat FRealFloat::Pi = FRealFloat(GenPi());
FRealFloat FRealFloat::HalfPi = FRealFloat(GenHalfPi());
FRealFloat FRealFloat::DegToRad = FRealFloat::Pi / 180_fl;

// Converts this number to a double number. Note that this can lead to huge precision loss
double FRealFloat::ToDouble() const
{
    return Value.ToFloat();
}

// Converts this number to a float number. Note that this can lead to huge precision loss
float FRealFloat::ToFloat() const
{
    return Value.ToFloat();
}

FString FRealFloat::ToString() const
{
    return Value.ToString().c_str();
}

bool FRealFloat::ExportTextItem(FString& ValueStr, FRealFloat const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
{
    ValueStr += FString::Printf(TEXT("(%s)"), *ToString());
    return true;
}

bool FRealFloat::ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText)
{
    FString MutableString = Buffer;
    const int32 Len = MutableString.Find(")") + 1;
    Buffer += Len;

    MutableString = MutableString.Left(Len);
    MutableString.RemoveFromStart("(");
    MutableString.RemoveFromEnd(")");

    *this = FRealFloat(MutableString);
    return true;
}

FRealFloat FRealFloat::GetMaxValue()
{
    return 0.00000000001_fl;
}

FRealFloat FRealFloat::GetMinValue()
{
    return 1e50_fl;
}

FRealFloat operator""_fl(const char* str)
{
    return FRealFloat(str);
}

FString URealFloatMath::ConvRealToString(FRealFloat InReal)
{
    return InReal.ToString();
}

float URealFloatMath::ConvRealToFloat(FRealFloat InVal)
{
    return InVal.ToFloat();
}

FRealFloat URealFloatMath::ConvStringToReal(const FString& InVal)
{
    return FRealFloat(InVal);
}

FRealFloat URealFloatMath::ConvFloatToReal(float InVal)
{
    return FRealFloat(InVal);
}

// Real math

FRealFloat URealFloatMath::RealPlusReal(FRealFloat First, FRealFloat Second)
{
    return FRealFloat(First.Value + Second.Value);
}

FRealFloat URealFloatMath::RealMinusReal(FRealFloat First, FRealFloat Second)
{
    return FRealFloat(First.Value - Second.Value);
}

FRealFloat URealFloatMath::RealMultReal(FRealFloat First, FRealFloat Second)
{
    return FRealFloat(First.Value * Second.Value);
}

FRealFloat URealFloatMath::RealDivReal(FRealFloat First, FRealFloat Second)
{
    return FRealFloat(First.Value / Second.Value);
}

bool URealFloatMath::RealEqualsReal(FRealFloat First, FRealFloat Second, FRealFloat Tolerance)
{
    return Abs(First - Second) < Tolerance;
}

bool URealFloatMath::RealNotEqualsReal(const FRealFloat& First, const FRealFloat& Second, const FRealFloat& Tolerance)
{
    return !URealFloatMath::RealEqualsReal(First, Second, Tolerance);
}

bool URealFloatMath::RealInfReal(FRealFloat First, FRealFloat Second)
{
    return First.Value < Second.Value;
}

bool URealFloatMath::RealInfEqReal(FRealFloat First, FRealFloat Second)
{
    return First.Value <= Second.Value;
}

bool URealFloatMath::RealSupReal(FRealFloat First, FRealFloat Second)
{
    return First.Value > Second.Value;
}

bool URealFloatMath::RealSupEqReal(FRealFloat First, FRealFloat Second)
{
    return First.Value >= Second.Value;
}

// Advanced FRealFloat math (trigo)

FRealFloat URealFloatMath::NormalizeAngleRad(FRealFloat InVal)
{
    const FRealFloat Intermediate = (InVal + FRealFloat::Pi) % (FRealFloat::Pi * 2_fl);
    return Intermediate + ((Intermediate < 0_fl) ? FRealFloat::Pi : -FRealFloat::Pi);
}

FRealFloat URealFloatMath::SinRad(FRealFloat InVal)
{
	return FRealFloat(ttmath::Sin(InVal.Value));
}

FRealFloat URealFloatMath::CosRad(FRealFloat InVal)
{
    return FRealFloat(ttmath::Cos(InVal.Value));
}

FRealFloat URealFloatMath::TanRad(FRealFloat InVal)
{
    return FRealFloat(ttmath::Tan(InVal.Value));
}

FRealFloat URealFloatMath::NormalizeAngleDeg(FRealFloat InVal)
{
    const FRealFloat Intermediate = (InVal + 180_fl) % 360_fl;
    return Intermediate + ((Intermediate < 0_fl) ? 180_fl : FRealFloat(-180.0));
}

FRealFloat URealFloatMath::SinDeg(FRealFloat InVal)
{
    return FRealFloat(ttmath::Sin((InVal * FRealFloat::DegToRad).Value));
}

FRealFloat URealFloatMath::CosDeg(FRealFloat InVal)
{
    return FRealFloat(ttmath::Cos((InVal * FRealFloat::DegToRad).Value));
}

FRealFloat URealFloatMath::TanDeg(FRealFloat InVal)
{
    return FRealFloat(ttmath::ATan((InVal * FRealFloat::DegToRad).Value));
}

FRealFloat URealFloatMath::AsinRad(FRealFloat InVal)
{
    return FRealFloat(ttmath::ASin(InVal.Value));
}

FRealFloat URealFloatMath::AcosRad(FRealFloat InVal)
{
    return FRealFloat(ttmath::ACos(InVal.Value));
}

FRealFloat URealFloatMath::AtanRad(FRealFloat InVal)
{
    return FRealFloat(ttmath::ATan(InVal.Value));
}

FRealFloat URealFloatMath::Atan2Rad(FRealFloat Y, FRealFloat X)
{
	// Undefined case, but we don't want to throw an exception, or to return NaN
    if (X == 0_fl && Y == 0_fl)
    {
        return 0_fl;
    }

    if (Y == 0_fl)
    {
        if (X > 0_fl)
        {
            return 0_fl;
        }
        else
        {
            return -FRealFloat::Pi;
        }
    }

    if (X == 0_fl)
    {
        if (Y > 0_fl)
        {
            return FRealFloat::HalfPi;
        }
        else
        {
            return -FRealFloat::HalfPi;
        }
    }

	FRealFloat Angle = AtanRad(Y / X);
    if (X < 0_fl)
	{
        Angle = NormalizeAngleRad(Angle + FRealFloat::Pi);
    }
    return Angle;
}

FRealFloat URealFloatMath::AsinDeg(FRealFloat InVal)
{
    return FRealFloat(ttmath::ASin(InVal.Value)) / FRealFloat::DegToRad;
}

FRealFloat URealFloatMath::AcosDeg(FRealFloat InVal)
{
    return FRealFloat(ttmath::ACos(InVal.Value)) / FRealFloat::DegToRad;
}

FRealFloat URealFloatMath::AtanDeg(FRealFloat InVal)
{
    return FRealFloat(ttmath::ATan(InVal.Value)) / FRealFloat::DegToRad;
}

FRealFloat URealFloatMath::Atan2Deg(FRealFloat Y, FRealFloat X)
{
    return Atan2Rad(Y, X) / FRealFloat::DegToRad;
}

// Advanced FRealFloat math

FRealFloat URealFloatMath::Pow(FRealFloat X, FRealFloat Y)
{
	// a^b = e^(b*ln(a))
    return FRealFloat(ttmath::Exp(Y.Value * ttmath::Ln(X.Value)));
}

FRealFloat URealFloatMath::Sqrt(FRealFloat Val)
{
    return FRealFloat(ttmath::Sqrt(Val.Value));
}

FRealFloat URealFloatMath::Exp(FRealFloat Val)
{
    return FRealFloat(ttmath::Exp(Val.Value));
}

FRealFloat URealFloatMath::LogE(FRealFloat Val)
{
    return FRealFloat(ttmath::Ln(Val.Value));
}

FRealFloat URealFloatMath::Log2(FRealFloat Val)
{
    return FRealFloat(ttmath::Log(Val.Value, FRealFloat(2).Value));
}

FRealFloat URealFloatMath::Log10(FRealFloat Val)
{
    return FRealFloat(ttmath::Log(Val.Value, FRealFloat(10).Value));
}

FRealFloat URealFloatMath::Min(FRealFloat First, FRealFloat Second)
{
    return FRealFloat(First < Second ? First : Second);
}

FRealFloat URealFloatMath::Max(FRealFloat First, FRealFloat Second)
{
    return FRealFloat(First > Second ? First : Second);
}

FRealFloat URealFloatMath::Clamp(FRealFloat Val, FRealFloat MinVal, FRealFloat MaxVal)
{
    return Max(Min(Val, MaxVal), MinVal);
}

FRealFloat URealFloatMath::Abs(FRealFloat Val)
{
    return FRealFloat(ttmath::Abs(Val.Value));
}

FRealFloat URealFloatMath::Sign(FRealFloat Val)
{
    return FRealFloat(Val < FRealFloat(0) ? 1 : -1);
}
