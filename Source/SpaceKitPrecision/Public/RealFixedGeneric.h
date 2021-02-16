// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

// Include ttmath
#pragma warning(push)
#pragma warning(disable: 5051)
#define TTMATH_NOASM // It doesn't seem to be possible to link to ASM in UE4, so, disable it
#include "SpaceKitPrecision/Private/ttmath/ttmath.h"
#pragma warning(pop)

#include "Core/Public/HAL/Platform.h"
#include "CoreMinimal.h"


// Helpers for pow big, as the default Pow function is inline
template<int a, int b>
constexpr ttmath::Big<a, b> PowBig(const ttmath::Big<a, b>& x, const ttmath::Big<a, b>& y)
{
	ttmath::Big<a, b> temp = x;
	temp.PowInt(y);
	return temp;
}

// Helpers for pow int, as the default Pow function is inline
template<int a>
constexpr ttmath::Int<a> PowInt(const ttmath::Int<a>& x, const ttmath::Int<a>& y)
{
	ttmath::Int<a> temp = x;
	temp.Pow(y);
	return temp;
}

// Type for a number with fixed point. MantissaSize is the size of the mantissa, in bits, and exponent is the (negated) 2-powered exponent of the number.
// Exponent has to be positive, as it is negated i.e. if the actual value is mantissa * 2^(-exponent).
// The actual mantissa size is guaranteed to be at least MantissaSize, but can actually be bigger.
// As the mantissa sized is increased by Exponent, so that it stays consistent during computations, the stored mantissa size will be actually be MantissaSize + Exponent.
// Ttmath ceils the numbers sizes to the nearest 64 multiple, so the actual mantissa size will be the nearest upper multiple of 64 of MantissaSize + Exponent.
template<int MantissaSize, int Exponent>
struct real_fixed
{
	using ttIntMantissaType = ttmath::Int<TTMATH_BITS(MantissaSize + Exponent)>; // Integer mantissa type
	using ttBigType = ttmath::Big<1, TTMATH_BITS(MantissaSize + Exponent)>; // Float type that can store the mantissa fully, without precision loss

	// The mantissa of this number
	ttIntMantissaType mantissa;

	// Cached value of the exponent. Aka these variables' values are 2^Exponent.
	// These are helpful for conversions and operations, and pre-computing them enhances performance
	static const ttIntMantissaType exponentiatedTtInt;
	static const ttBigType exponentiatedTtBig;
	static const double exponentiatedDouble;
	
	// Default constructor
	constexpr real_fixed()
	{
	}

	// Create a copy of a given real_fixed number
	constexpr real_fixed(const real_fixed<MantissaSize, Exponent>& x)
		: mantissa(x.mantissa)
	{
	}

private:

	// Do not use. Use real_fixed::FromMantissa instead, that explicitly describes this
	constexpr real_fixed(const ttIntMantissaType& inMantissa)
		: mantissa(inMantissa)
	{
	}

public:

	// Builds a fixed-size value using a given mantissa.
	// Note the difference between the mantissa and the value: this does not build a number that value is x, but that values is x*2^-Exponent
	static real_fixed<MantissaSize, Exponent> FromMantissa(const ttIntMantissaType& inMantissa)
	{
		return real_fixed(inMantissa);
	}
	
	// Creates a real_fixed number based on a ttmath float number
	constexpr real_fixed(ttBigType inValue)
	{
		(inValue * exponentiatedTtBig).ToInt(mantissa);
	}

	// Creates a real_fixed number based on a base-10 string representation
	constexpr real_fixed(const std::string& initString)
	{
		(ttBigType(initString) * exponentiatedTtBig).ToInt(mantissa);
	}

	// See real_fixed(std::string initString)
	constexpr real_fixed(const char* initString)
	{
		(ttBigType(initString) * exponentiatedTtBig).ToInt(mantissa);
	}

	// See real_fixed(std::string initString)
	constexpr real_fixed(const FString& initString)
	{
		(ttBigType(TCHAR_TO_ANSI(*initString)) * exponentiatedTtBig).ToInt(mantissa);
	}

	// Creates a real_fixed number based on a double number
	constexpr real_fixed(double val)
	{
		(ttBigType(val) * exponentiatedDouble).ToInt(mantissa);
	}

	// Creates a real_fixed number based on a float number
	constexpr real_fixed(float val)
	{
		(ttBigType(val) * exponentiatedDouble).ToInt(mantissa);
	}

	// Creates a real_fixed number based on an 32-bits integer number
	constexpr real_fixed(int32 val)
	{
		(ttBigType(val) * exponentiatedDouble).ToInt(mantissa);
	}

	// Creates a real_fixed number based on an 64-bits integer number
	constexpr real_fixed(int64 val)
	{
		(ttBigType(val) * exponentiatedDouble).ToInt(mantissa);
	}

	// Converts this number to a double number. Note that this can lead to huge precision loss
	constexpr double ToDouble() const
	{
		return ToBig().ToDouble();
	}

	// Converts this number to a float number. Note that this can lead to huge precision loss
	constexpr float ToFloat() const
	{
		return ToBig().ToFloat();
	}

	// Converts this number to a floating-point big number. This may not lead to precision loss
	constexpr ttBigType ToBig() const
	{
		return ttBigType(mantissa) / exponentiatedDouble;
	}

	// Converts this number to a base 10 string, with little to no precision loss
	// The precision loss appears for small numbers, where the string is rounded:
	// For a given x, then |x - real_fixed(x.ToString())| <= 10^(-floor(Exponent*ln(2)/ln(10)))
	// However, if x is itself imported from an exported string, there will be no precision loss:
	// For a given y, so that x=real_fixed(y.ToString()), then x = real_fixed(x.ToString())
	FString ToString() const
	{
		// There is no built-in way to convert fixed point numbers to string, and it's too hard and error prone to code it manually,
		// so we'll use some of the features ttmath has to offer, but with some modifications

		// First, extract the integral (left side of comma) and decimal (right side of the comma) parts
		const ttBigType RightPart = ttBigType(Abs(mantissa) % exponentiatedTtInt) / exponentiatedTtBig;
		const ttIntMantissaType LeftPart = mantissa / exponentiatedTtInt;

		// Convert the right side to string, using ttmath.
		ttmath::Conv c;
		// Also, tell ttmath about the maximum figures, so we don't get rounding errors (aka 0.1 that becomes 0.099999999)
		c.round = FMath::FloorToInt((float)Exponent * FMath::Loge(2.0) / FMath::Loge(10.0));
		FString RightPartString = FString(RightPart.ToString(c).c_str());
		// Remove the "0." part, so we can append this to the left part, adding a comma in between
		RightPartString.RemoveFromStart("0.");

		// Convert the left side to string, using ttmath.
		const FString LeftPartString = ttbigToString(LeftPart);

		// Compute the result, and return
		const FString Result = LeftPartString + "." + RightPartString;
		return Result;
	}

	static real_fixed<MantissaSize, Exponent> GetMaxValue()
	{
		ttIntMantissaType temp;
		temp.SetMax();
		return real_fixed<MantissaSize, Exponent>(temp);
	}

	static real_fixed<MantissaSize, Exponent> GetMinValue()
	{
		return real_fixed<MantissaSize, Exponent>::FromMantissa(real_fixed<MantissaSize, Exponent>::ttIntMantissaType(1));
	}
};

// Helper to turn a ttmath number (integer or float) into an FString
template<typename T>
FString ttbigToString(T x)
{
	return FString(x.ToString().c_str());
}

// Definitions exponentiatedTtInt, exponentiatedTtBig and exponentiatedDouble (Cached value of the exponent). See the declarations for more info
template<int MantissaSize, int Exponent>
const ttmath::Int<TTMATH_BITS(MantissaSize + Exponent)> real_fixed<MantissaSize, Exponent>::exponentiatedTtInt = PowInt(ttIntMantissaType(2), ttIntMantissaType(Exponent));

template<int MantissaSize, int Exponent>
const ttmath::Big<1, TTMATH_BITS(MantissaSize + Exponent)> real_fixed<MantissaSize, Exponent>::exponentiatedTtBig = PowBig(ttBigType(2.0), ttBigType(Exponent));

template<int MantissaSize, int Exponent>
const double real_fixed<MantissaSize, Exponent>::exponentiatedDouble = pow(2.0, Exponent);

// Operators for fixed point numbers

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator+(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return real_fixed<MantissaSize, Exponent>::FromMantissa(x.mantissa + y.mantissa);
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator+=(real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return real_fixed<MantissaSize, Exponent>::FromMantissa(x.mantissa += y.mantissa);
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator+=(real_fixed<MantissaSize, Exponent>& x, const float& y)
{
	return x += real_fixed<MantissaSize, Exponent>(y);
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator-(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return real_fixed<MantissaSize, Exponent>::FromMantissa(x.mantissa - y.mantissa);
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator-=(real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return real_fixed<MantissaSize, Exponent>::FromMantissa(x.mantissa -= y.mantissa);
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator-(const real_fixed<MantissaSize, Exponent>& x)
{
	return real_fixed<MantissaSize, Exponent>::FromMantissa(-x.mantissa);
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator*(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return real_fixed<MantissaSize, Exponent>::FromMantissa((x.mantissa * y.mantissa) / real_fixed<MantissaSize, Exponent>::exponentiatedTtInt);
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator*=(real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return x = x * y;
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator/(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return real_fixed<MantissaSize, Exponent>::FromMantissa((x.mantissa * real_fixed<MantissaSize, Exponent>::exponentiatedTtInt) / y.mantissa);
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator/=(real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return x = x / y;
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator%(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return real_fixed<MantissaSize, Exponent>::FromMantissa(x.mantissa % y.mantissa);
}

template<int MantissaSize, int Exponent>
real_fixed<MantissaSize, Exponent> operator%=(real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return x = x % y;
}

template<int MantissaSize, int Exponent>
bool operator<(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return x.mantissa < y.mantissa;
}

template<int MantissaSize, int Exponent>
bool operator<=(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return x.mantissa <= y.mantissa;
}

template<int MantissaSize, int Exponent>
bool operator>=(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return x.mantissa >= y.mantissa;
}

template<int MantissaSize, int Exponent>
bool operator>(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return x.mantissa > y.mantissa;
}

template<int MantissaSize, int Exponent>
bool operator==(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return x.mantissa == y.mantissa;
}

template<int MantissaSize, int Exponent>
bool operator!=(const real_fixed<MantissaSize, Exponent>& x, const real_fixed<MantissaSize, Exponent>& y)
{
	return x.mantissa != y.mantissa;
}
