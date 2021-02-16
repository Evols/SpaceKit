// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "SpaceKitPrecision/Public/RealFixed.h"
#include "RealFloat.h"

FRealFixed::FRealFixed()
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = 0.0;
}

FRealFixed::FRealFixed(const FRealFixed& InValue)
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = InValue.Value;
}

FRealFixed::FRealFixed(const real_fixed_type& InValue)
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = InValue;
}

FRealFixed::FRealFixed(int32 InValue)
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = InValue;
}

FRealFixed::FRealFixed(int64 InValue)
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = InValue;
}

FRealFixed::FRealFixed(float InValue)
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = InValue;
}

FRealFixed::FRealFixed(double InValue)
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = InValue;
}
FRealFixed::FRealFixed(const char* InValue)
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = InValue;
}

FRealFixed::FRealFixed(const std::string& InValue)
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = InValue;
}

FRealFixed::FRealFixed(const FString& InValue)
    : Value(*reinterpret_cast<real_fixed_type*>(InternalValue))
{
    Value = InValue;
}

// Converts this number to a double number. Note that this can lead to huge precision loss
double FRealFixed::ToDouble() const
{
    return Value.ToDouble();
}

// Converts this number to a float number. Note that this can lead to huge precision loss
float FRealFixed::ToFloat() const
{
    return Value.ToFloat();
}

// Converts this number to a floating-point big number. This may not lead to precision loss
real_fixed_type::ttBigType FRealFixed::ToBig() const
{
    return Value.ToBig();
}

FString FRealFixed::ToString() const
{
    return Value.ToString();
}

bool FRealFixed::ExportTextItem(FString& ValueStr, FRealFixed const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
{
    ValueStr += FString::Printf(TEXT("(%s)"), *ToString());
    return true;
}

bool FRealFixed::ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText)
{
    FString MutableString = Buffer;
    const int32 Len = MutableString.Find(")") + 1;
    Buffer += Len;

    MutableString = MutableString.Left(Len);
    MutableString.RemoveFromStart("(");
    MutableString.RemoveFromEnd(")");

    Value = MutableString;
    return true;
}


// Conversions

FString URealFixedMath::ConvRealToString(const FRealFixed& InReal)
{
    return FString::Printf(TEXT("%s"), *InReal.ToString());
}

float URealFixedMath::ConvRealToFloat(const FRealFixed& Val)
{
    return Val.Value.ToFloat();
}

FRealFixed URealFixedMath::ConvStringToReal(const FString& InString)
{
    return FRealFixed(InString);
}

FRealFixed URealFixedMath::ConvFloatToReal(float Val)
{
    return FRealFixed(Val);
}

// Real math

FRealFixed URealFixedMath::RealPlusReal(const FRealFixed& First, const FRealFixed& Second)
{
    return First + Second;
}

FRealFixed URealFixedMath::RealMinusReal(const FRealFixed& First, const FRealFixed& Second)
{
    return First - Second;
}

FRealFixed URealFixedMath::RealMultReal(const FRealFixed& First, const FRealFixed& Second)
{
    return First * Second;
}

FRealFixed URealFixedMath::RealDivReal(const FRealFixed& First, const FRealFixed& Second)
{
    return First / Second;
}

FRealFixed URealFixedMath::RealModReal(const FRealFixed& First, const FRealFixed& Second)
{
    return First % Second;
}

bool URealFixedMath::RealEqualsReal(const FRealFixed& First, const FRealFixed& Second, const FRealFixed& Tolerance)
{
    return URealFixedMath::Abs(First - Second) < Tolerance;
}

bool URealFixedMath::RealNotEqualsReal(const FRealFixed& First, const FRealFixed& Second, const FRealFixed& Tolerance)
{
    return !URealFixedMath::RealEqualsReal(First, Second, Tolerance);
}

bool URealFixedMath::RealInfReal(const FRealFixed& First, const FRealFixed& Second)
{
    return First < Second;
}

bool URealFixedMath::RealInfEqReal(const FRealFixed& First, const FRealFixed& Second)
{
    return First <= Second;
}

bool URealFixedMath::RealSupReal(const FRealFixed& First, const FRealFixed& Second)
{
    return First > Second;
}

bool URealFixedMath::RealSupEqReal(const FRealFixed& First, const FRealFixed& Second)
{
    return First >= Second;
}

// Advanced FRealFixed math

FRealFixed URealFixedMath::Sqrt(const FRealFixed& Val)
{
    return FRealFixed(ttmath::Sqrt((Val.ToBig())));
}

FRealFixed URealFixedMath::LogE(const FRealFixed& Val)
{
    return FRealFixed(ttmath::Ln(Val.ToBig()));
}

FRealFixed URealFixedMath::Log2(const FRealFixed& Val)
{
    return FRealFixed(ttmath::Log(Val.ToBig(), real_fixed_type::ttBigType(2)));
}

FRealFixed URealFixedMath::Log10(const FRealFixed& Val)
{
    return FRealFixed(ttmath::Log(Val.ToBig(), real_fixed_type::ttBigType(10)));
}

FRealFixed URealFixedMath::Min(const FRealFixed& Val, const FRealFixed& InMin)
{
    return FRealFixed(Val > InMin ? Val : InMin);
}

FRealFixed URealFixedMath::Max(const FRealFixed& Val, const FRealFixed& InMax)
{
    return FRealFixed(Val > InMax ? Val : InMax);
}

FRealFixed URealFixedMath::Clamp(const FRealFixed& Val, const FRealFixed& InMin, const FRealFixed& InMax)
{
    return Max(Min(Val, InMin), InMax);
}

FRealFixed URealFixedMath::Abs(const FRealFixed& Val)
{
	return Val < 0_fx ? -Val : Val;
}

FRealFixed operator""_fx(const char* str)
{
    return FRealFixed(str);
}
